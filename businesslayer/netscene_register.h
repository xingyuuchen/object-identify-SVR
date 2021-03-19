#ifndef OI_SVR_NETSCENE_REGISTER_H
#define OI_SVR_NETSCENE_REGISTER_H
#include "netscenebase.h"
#include "dbitem.h"


class NetSceneRegister : public NetSceneBase {
  public:
    NetSceneRegister();
    
    int GetType() override;
    
    NetSceneBase *NewInstance() override;
    
    int DoSceneImpl(const std::string &_in_buffer) override;

  private:
  
};


class RowUserInfo : public DBItem {
  public:

    explicit RowUserInfo(std::string _nickname, std::string _avatar_path);

    const char *table() const override;
    
    void PopulateFieldList(std::vector<std::string> &_filed_list) const override;
    
    void PopulateValueList(std::vector<std::string> &_value_list) const override;
    
    uint64_t GetUsrId() const;

  private:
    static const char * table_;
    static const char * field_name1_;
    static const char * field_name2_;
    uint64_t            usr_id_;
    std::string         nickname_;
    std::string         avatar_path_;
};


#endif //OI_SVR_NETSCENE_REGISTER_H
