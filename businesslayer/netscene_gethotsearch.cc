#include "netscene_gethotsearch.h"
#include "netscenetypes.h"
#include "log.h"
#include "timeutil.h"
#include "threadpool/threadpool.h"
#include "connection.h"
#include "dbitem_recognition.h"
#include <queue>



/* recompute hot-search every 2min */
const int NetSceneGetHotSearch::hot_search_refresh_period_ = 2 * 60 * 1000;
//const int NetSceneGetHotSearch::hot_search_refresh_period_ = 10 * 1000;

const int NetSceneGetHotSearch::hot_search_max_cnt_ = 9;

std::map<std::string, int> NetSceneGetHotSearch::item_frequency_map_;

std::vector<NetSceneGetHotSearch::HotSearchItem>
        NetSceneGetHotSearch::hot_searches_;

std::mutex NetSceneGetHotSearch::hot_search_mutex_;


NetSceneGetHotSearch::NetSceneGetHotSearch()
        : NetSceneBase() {
    
    NETSCENE_INIT_START
        ThreadPool::Instance().ExecutePeriodic(
                hot_search_refresh_period_, [] { __ComputeHotSearch(); });
    NETSCENE_INIT_END
    
}

int NetSceneGetHotSearch::DoSceneImpl(const std::string &_in_buffer) {
    LogI(__FILE__, "[DoSceneImpl] req.len: %zd", _in_buffer.size());
    if (socket_ <= 0) {
        LogE(__FILE__, "[DoSceneImpl] Socket NOT open");
        return -1;
    }
    NetSceneGetHotSearchProto::NetSceneGetHotSearchReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    LogI(__FILE__, "[DoSceneImpl] usr_id: %d", usr_id)
    
    
    NetSceneGetHotSearchProto::NetSceneGetHotSearchResp resp;
    
    __PopulateHotSearchResp(resp);
    
    size_t size = resp.ByteSizeLong();
    std::string byte_string;
    resp.SerializeToString(&byte_string);
    Write2BaseResp(byte_string, size);
    return 0;
}

void NetSceneGetHotSearch::__ComputeHotSearch() {
    LogI(__FILE__, "[__ComputeHotSearch] start")
    uint64_t start = ::gettickcount();
    
    item_frequency_map_.clear();
    hot_searches_.clear();
    
    std::vector<std::string> query_res;
    char sql[128] = {0, };
    snprintf(sql, sizeof(sql), "select %s, %s from %s;",
             DBItem_Recognition::field_name_item_name_,
             DBItem_Recognition::field_name_item_type_,
             DBItem_Recognition::table_);
    int db_ret = Dao::Query(sql, query_res, 2);
    if (db_ret < 0) {
        LogE(__FILE__, "db query failed.")
        return;
    }
    
    LogI(__FILE__, "[__ComputeHotSearch] db query succeed, cnt: %lu", query_res.size())
    
    for (auto it = query_res.begin(); it != query_res.end(); ) {
        std::string item_name = *it;
        ++it;
        if (it == query_res.end()) {
            LogE(__FILE__, "[__ComputeHotSearch] wtf???")
            break;
        }
        int type = it->c_str()[0] - 0x30;
        if (type < 0 || type > 2) {
            LogE(__FILE__, "[__ComputeHotSearch] name: %s, type: %d",
                 item_name.c_str(), type)
            type = 0;
        }
        ++it;
        
        // TODO: optimize
        item_name.append(std::to_string(type));
        ++item_frequency_map_[item_name];
    }
    
    LogI(__FILE__, "[__ComputeHotSearch] frequency computed, %lu different search",
            item_frequency_map_.size())
    
    using ItemType = NetSceneGetHotSearchProto::
            NetSceneGetHotSearchResp_HotSearchItem_ItemType;
    
    std::priority_queue<NetSceneGetHotSearch::HotSearchItem,
            std::vector<NetSceneGetHotSearch::HotSearchItem>,
            NetSceneGetHotSearch::HotSearchItemComparator> priority_queue;
    
    for (auto &it : item_frequency_map_) {
        HotSearchItem item;
        item.set_heat(it.second);
        int type = it.first.at(it.first.size() - 1) - 0x30;
        type = (type < 0 || type > 2) ? 0 : type;
        item.set_item_type((ItemType) type);
        item.set_item_name(it.first.substr(0, it.first.size() - 1));
        priority_queue.emplace(item);
    }
    
    LogI(__FILE__, "[__ComputeHotSearch] priority queue size: %ld",
         priority_queue.size())
         
    {
        std::unique_lock<std::mutex> lock(hot_search_mutex_);
        for (int i = 0; i < hot_search_max_cnt_; ++i) {
            if (priority_queue.empty()) {
                break;
            }
            hot_searches_.emplace_back(priority_queue.top());
            priority_queue.pop();
        }
        LogI(__FILE__, "[__ComputeHotSearch] hot_searches size: %ld",
             hot_searches_.size())
    }
    
    uint64_t cost = ::gettickcount() - start;
    LogI(__FILE__, "[__ComputeHotSearch] done, cost time: %llu ms", cost)
}

void NetSceneGetHotSearch::__PopulateHotSearchResp(
        NetSceneGetHotSearchProto::NetSceneGetHotSearchResp &_resp) {
    HotSearchItem *hot_search_item;
    
    std::unique_lock<std::mutex> lock(hot_search_mutex_);
    if (hot_searches_.empty()) {
        LogI(__FILE__, "[__PopulateHotSearch] hot_search is empty.")
        return;
    }
    for (auto &it : hot_searches_) {
        hot_search_item = _resp.add_hot_search_items();
        hot_search_item->CopyFrom(it);
    }
    LogI(__FILE__, "[__PopulateHotSearch] copied %ld items", hot_searches_.size())
}

int NetSceneGetHotSearch::__GetFrequency(std::string &_item_name) {
    auto find = item_frequency_map_.find(_item_name);
    if (find != item_frequency_map_.end()) {
        return find->second;
    }
    return 0;
}

/**
 * debug only
 */
void NetSceneGetHotSearch::__ShowFrequencyMap() {
    LogI(__FILE__, "[__ShowFrequencyMap] map size: %lu",
         item_frequency_map_.size())
    for (auto &it : item_frequency_map_) {
        LogI(__FILE__, "[__ShowFrequencyMap] %s : %d",
             it.first.c_str(), it.second)
    }
}

int NetSceneGetHotSearch::GetType() { return kNetSceneTypeGetHotSearch; }

NetSceneBase *NetSceneGetHotSearch::NewInstance() { return new NetSceneGetHotSearch(); }


bool NetSceneGetHotSearch::HotSearchItemComparator::operator()(
        const NetSceneGetHotSearch::HotSearchItem &_lhs,
        const NetSceneGetHotSearch::HotSearchItem &_rhs) {
    return _lhs.heat() > _rhs.heat();
}
