/**
 * @file tar_get_task.h
 * @author Likun Liu <liulikun@gmail.com>
 *
 * @brief tar get task2.
 *
 * Copyright (C) 2009 Likun Liu <liulikun@gmail.com>
 * Free Software License:
 *
 * All rights are reserved by the author, with the following exceptions:
 * Permission is granted to freely reproduce and distribute this software,
 * possibly in exchange for a fee, provided that this copyright notice appears
 * intact. Permission is also granted to adapt this software to produce
 * derivative works, as long as the modified versions carry this copyright
 * notice and additional notices stating that the work has been modified.
 * This source code may be translated into executable form and incorporated
 * into proprietary software; there is no requirement for such software to
 * contain a copyright notice related to this source.
 *
 * $Id: $
 * $Name: $
 */

#ifndef TAR_DOWNLOAD_TASK_H
#define TAR_DOWNLOAD_TASK_H

#include "zisync/kernel/libevent/transfer_task.h"

namespace zs {

class DownloadTask2 : public TransferTask
                    , public IDownloadTask
                    , public ITarParserDelegate
                    , public ITarParserDataSource {
 public:
  DownloadTask2(ITaskMonitor* monitor,
              TransferServer2* server,
              int32_t local_tree_id,
              const std::string& remote_tree_uuid,
              const std::string& uri);
  virtual ~DownloadTask2();
  //
  // Implment IGetTask
  //
  virtual int GetTaskId() {
    return task_id_;
  }
  virtual err_t AppendFile(
      const std::string& encode_path,
      const std::string &target_path, int64_t size);
  virtual err_t Execute();

  //
  // Implement TransferTask
  //
  virtual void CreateResponseBodyParser(int32_t http_code);
  virtual void RequestHeadWriteAll(struct bufferevent* bev);
  virtual err_t RequestBodyWriteSome(struct bufferevent* bev);

  //
  // Implement ITarParserDelegate
  //
  virtual void OnTarWillTransfer(const std::string& tmp_dir);
  virtual void OnTarDidTransfer(const std::string& tmp_dir);

  virtual void OnFileWillTransfer(
      const std::string& real_path,
      const std::string& encode_path);

  virtual void OnFileDidTransfered(
      const std::string& real_path,
      const std::string& encode_path,
      const std::string& sha1);
  virtual void OnFileDidSkiped(
      const std::string& real_path,
      const std::string& encode_path);
  
  virtual void OnByteDidTransfered(
      const std::string& real_path,
      const std::string& encode_path, int32_t nbytes);
  //
  // Implement ITarParserDataSource
  //
  virtual std::string GetRealPath(const std::string& encode_path);
  virtual std::string GetAlias(const std::string& encode_path);

  //
  // Override some TransferTask method
  //
  virtual void OnComplete(struct bufferevent* bev, err_t eno);
  
 private:
  std::string local_tree_uuid_;

  int64_t total_size_;
  std::string encode_path_;
  std::string target_path_;

  ITaskMonitor* monitor_;
};

}  // namespace zs


#endif
