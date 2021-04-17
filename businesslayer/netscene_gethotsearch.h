#pragma once
#include "netsceneprotobuf.h"
#include <map>
#include <vector>
#include <mutex>
#include "netscenegethotsearch.pb.h"


class NetSceneGetHotSearch : public NetSceneProtobuf {
  public:
    
    NetSceneGetHotSearch();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    RespMessage *GetRespMessage() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
    
    using HotSearchItem = NetSceneGetHotSearchProto::
                NetSceneGetHotSearchResp_HotSearchItem;
    
    static void __ComputeHotSearch();
    
    void __PopulateHotSearchResp();
    
    static int __GetFrequency(std::string &_item_name);
    
    /**
     * debug only
     */
    static void __ShowFrequencyMap();
    
    
    struct HotSearchItemComparator {
        bool operator()(const HotSearchItem &_lhs,
                        const HotSearchItem &_rhs);
    };
    
    
  private:
    NetSceneGetHotSearchProto::NetSceneGetHotSearchResp resp_;
    static const int                        hot_search_refresh_period_;
    static const int                        hot_search_max_cnt_;
    static std::map<std::string, int>       item_frequency_map_;
    static std::vector<HotSearchItem>       hot_searches_;
    static std::mutex                       hot_search_mutex_;
    
};

