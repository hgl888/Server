//
//  transfer_task_manager.hpp
//  Server
//
//  Created by mac on 16/9/30.
//  Copyright © 2016年 mac. All rights reserved.
//

#ifndef FILE_SERVER_TRANSFER_TASK_MANAGER_H_
#define FILE_SERVER_TRANSFER_TASK_MANAGER_H_

#include <stdio.h>
#include "singleton.h"
#include "transfer_task.h"


class TransferTaskManager : public Singleton<TransferTaskManager>
{
public:
    ~TransferTaskManager();
    
    void OnTimer(uint64_t tick);
    
    BaseTransferTask* NewTransferTask(uint32_t trans_mode, const std::string& task_id,
        uint32_t from_user_id, uint32_t to_user_id, const std::string& file_name, uint32_t file_size);
    
    OfflineTransferTask* NewTransferTask(const std::string& task_id, uint32_t to_user_id);
    
    bool DeleteTransferTask(const std::string& task_id);
    bool DeleteTransferTaskByConnClose(const std::string& task_id);
    
    BaseTransferTask* FindByTaskID(const std::string& task_id)
    {
        BaseTransferTask* transfer_task = NULL;
        
        TransferTaskMap::iterator it = transfer_tasks_.find(task_id);
        if (it != transfer_tasks_.end())
        {
            transfer_task = it->second;
        }
        
        return transfer_task;
    }
    
private:
    friend class Singleton<TransferTaskManager>;
    
    TransferTaskManager();
    
    TransferTaskMap transfer_tasks_;
};

#endif /* transfer_task_manager_hpp */
