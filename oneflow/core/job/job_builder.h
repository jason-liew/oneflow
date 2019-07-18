#ifndef ONEFLOW_CORE_JOB_JOB_CONF_BUILDER_H_
#define ONEFLOW_CORE_JOB_JOB_CONF_BUILDER_H_

#include "oneflow/core/job/job_desc.h"
#include "oneflow/core/register/op_blob_arg.pb.h"

namespace oneflow {

void SetBnValInOpTypeConf(PbMessage* pb_msg, const std::string& bn, const std::string& old_val,
                          const std::string& new_val);

class SbpParallel;
class LogicalBlobId;
class Operator;

class JobBuilder final {
 public:
  OF_DISALLOW_COPY_AND_MOVE(JobBuilder);
  explicit JobBuilder(Job* job);
  ~JobBuilder() = default;
  
  Job *mutable_job() { return job_; }
  const Job& job() const { return *job_; }

  void AddOps(const ParallelConf& parallel_conf, const std::vector<OperatorConf>& op_confs);
  void MutOps(const std::vector<OperatorConf>& op_confs) const;
  void AddOrMutOps(const ParallelConf& parallel_conf, const std::vector<OperatorConf>& op_confs);
  void RemoveOp(const std::string &op_name);

  SbpParallel* MutSbpParallel4Oba(const OpBlobArg& oba) const;
  void BindIdenticalSbpOpBlobArgPair(const OpBlobArg& first, const OpBlobArg& second);

  void ForEachOperator(const std::function<void(const Operator&)>& Handler) const;

  OperatorConf *MutableOpConf(const std::string &op_name);
  const OperatorConf &OpConf(const std::string &op_name);

  SbpSignature *MutableOpSbpSignature(const std::string &op_name);
  const SbpSignature& OpSbpSignature(const std::string &op_name) const;
  void AddOpSbpSignature(const std::string &op_name,
                         const SbpSignature &sbp_signature);

  const ParallelConf &OpParallelConf(const std::string &op_name) const;
  ParallelConf *MutableOpParallelConf(const std::string &op_name);
  void AddOpParallelConf(const std::string &op_name, const ParallelConf &parallel_conf);

  const HashSet<LogicalBlobId> &batch_dim_lbis() const { return batch_dim_lbis_; }

 private:
  Job* job_;
  HashMap<std::string, OperatorConf*> op_name2op_conf_;
  HashMap<std::string, ParallelConf*> op_name2parallel_conf_;
  HashMap<std::string, SbpSignature*> op_name2sbp_signature_conf_;
  HashSet<LogicalBlobId> batch_dim_lbis_;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_JOB_JOB_CONF_BUILDER_H_
