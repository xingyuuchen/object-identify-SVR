#include "netscene_gethotsearch.h"
#include "netscene_queryimg/netscene_queryimg.h"
#include "netscenetypes.h"
#include "log.h"
#include "timeutil.h"
#include "threadpool/threadpool.h"
#include "connection.h"
#include "dbitem_recognition.h"
#include <queue>
#include <cstdlib>



/* recompute hot-search every minute */
const int NetSceneGetHotSearch::hot_search_refresh_period_ = 1 * 60 * 1000;
//const int NetSceneGetHotSearch::hot_search_refresh_period_ = 10 * 1000;   // debug

const int NetSceneGetHotSearch::hot_search_max_cnt_ = 9;

std::map<std::string, int> NetSceneGetHotSearch::item_frequency_map_;

std::vector<NetSceneGetHotSearch::HotSearchItem>
        NetSceneGetHotSearch::hot_searches_;

std::mutex NetSceneGetHotSearch::hot_search_mutex_;


NetSceneGetHotSearch::NetSceneGetHotSearch()
        : NetSceneProtobuf() {
    
    NETSCENE_INIT_START
        ThreadPool::Instance().ExecutePeriodic(
                hot_search_refresh_period_, [] { __ComputeHotSearch(); });
    NETSCENE_INIT_END
    
}

int NetSceneGetHotSearch::DoSceneImpl(const std::string &_in_buffer) {
    LogI("req.len: %zd", _in_buffer.size());
    
    NetSceneGetHotSearchProto::NetSceneGetHotSearchReq req;
    req.ParseFromArray(_in_buffer.data(), _in_buffer.size());
    uint32_t usr_id = req.usr_id();
    LogI("usr_id: %d", usr_id)
    
    __PopulateHotSearchResp();
    
    return 0;
}

void NetSceneGetHotSearch::__ComputeHotSearch() {
    uint64_t start = ::gettickcount();
    if (start - NetSceneQueryImg::GetLastQueryTs() > 2 * hot_search_refresh_period_) {
        return;
    }
    
    item_frequency_map_.clear();
    hot_searches_.clear();
    
    std::vector<std::string> query_res;
    char sql[256] = {0, };
    snprintf(sql, sizeof(sql), "select %s from %s;",
             DBItem_Recognition::field_name_item_name_,
             DBItem_Recognition::table_);
    int db_ret = Dao::Query(sql, query_res, 1);
    if (db_ret < 0) {
        LogE("db query failed.")
        return;
    }
    
    LogI("db query succeed, cnt: %lu", query_res.size())
    
    for (const auto& item_name : query_res) {
        ++item_frequency_map_[item_name];
    }
    
    LogI("frequency computed, %lu different search",
            item_frequency_map_.size())
    
    using ItemType = NetSceneGetHotSearchProto::
            NetSceneGetHotSearchResp_HotSearchItem_ItemType;
    
    std::priority_queue<NetSceneGetHotSearch::HotSearchItem,
            std::vector<NetSceneGetHotSearch::HotSearchItem>,
            NetSceneGetHotSearch::HotSearchItemComparator> priority_queue;
    
    for (auto &it : item_frequency_map_) {
        HotSearchItem item;
        int heat_factor = rand() % 30 + 20;
        item.set_heat(it.second * heat_factor);
    
        query_res.clear();
        snprintf(sql, sizeof(sql), "select %s, %s from %s where %s='%s' limit 1",
                 DBItem_Recognition::field_name_item_type_,
                 DBItem_Recognition::field_name_item_desc_,
                 DBItem_Recognition::table_,
                 DBItem_Recognition::field_name_item_name_,
                 it.first.c_str());
        db_ret = Dao::Query(sql, query_res, 2);
        if (db_ret < 0) {
            LogE("query desc failed")
            return;
        }
        if (query_res.size() != 2) {
            LogE("wtf??, %lu", query_res.size())
            continue;
        }
        
        int type = query_res[0].c_str()[0] - 0x30;
        if (type < 0 || type > 2) {
            LogE("type err: name: %s, type: %d",
                 it.first.c_str(), type)
            type = 0;
        }
    
        item.set_item_name(it.first);
        item.set_item_type((ItemType) type);
        item.set_item_desc(query_res[1]);
        
        priority_queue.emplace(item);
    }
    
    {
        std::unique_lock<std::mutex> lock(hot_search_mutex_);
        for (int i = 0; i < hot_search_max_cnt_; ++i) {
            if (priority_queue.empty()) {
                break;
            }
            hot_searches_.emplace_back(priority_queue.top());
            priority_queue.pop();
        }
    }
    
    uint64_t cost = ::gettickcount() - start;
    LogI("done, cost time: %llu ms", cost)
}

void NetSceneGetHotSearch::__PopulateHotSearchResp() {
    HotSearchItem *hot_search_item;
    
    std::unique_lock<std::mutex> lock(hot_search_mutex_);
    if (hot_searches_.empty()) {
        LogI("hot_search is empty.")
        return;
    }
    for (auto &it : hot_searches_) {
        hot_search_item = resp_.add_hot_search_items();
        hot_search_item->CopyFrom(it);
    }
    LogI("copied %ld items", hot_searches_.size())
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
    LogI("map size: %lu",
         item_frequency_map_.size())
    for (auto &it : item_frequency_map_) {
        LogI("%s : %d",
             it.first.c_str(), it.second)
    }
}

int NetSceneGetHotSearch::GetType() { return kNetSceneTypeGetHotSearch; }

NetSceneBase *NetSceneGetHotSearch::NewInstance() { return new NetSceneGetHotSearch(); }

NetSceneProtobuf::RespMessage *NetSceneGetHotSearch::GetRespMessage() { return &resp_; }

bool NetSceneGetHotSearch::HotSearchItemComparator::operator()(
        const NetSceneGetHotSearch::HotSearchItem &_lhs,
        const NetSceneGetHotSearch::HotSearchItem &_rhs) {
    return _lhs.heat() < _rhs.heat();
}
