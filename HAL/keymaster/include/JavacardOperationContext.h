/*
 **
 ** Copyright 2020, The Android Open Source Project
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **     http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 */

#ifndef KEYMASTER_V4_1_JAVACARD_OPERATIONCONTEXT_H_
#define KEYMASTER_V4_1_JAVACARD_OPERATIONCONTEXT_H_

#include <iostream>
#include <android/hardware/keymaster/4.1/IKeymasterDevice.h>

#define MAX_BUF_SIZE 32

namespace keymaster {
namespace V4_1 {
namespace javacard {

using ::android::hardware::keymaster::V4_0::ErrorCode;
using ::android::hardware::keymaster::V4_0::Algorithm;
using ::android::hardware::keymaster::V4_0::KeyPurpose;

using sendDataToSE_cb = std::function<ErrorCode(std::vector<uint8_t>& data)>;

enum class Operation;
struct OperationData;

struct OperationInfo {
    Algorithm alg;
    KeyPurpose purpose;
};

class OperationContext {

public:
    OperationContext(){}
    ~OperationContext() {}
    ErrorCode setOperationData(uint64_t operationHandle, OperationInfo& oeprInfo);
    ErrorCode getOperationData(uint64_t operHandle, OperationInfo& operInfo);
    ErrorCode clearOperationData(uint64_t operationHandle);
    ErrorCode update(uint64_t operHandle, std::vector<uint8_t>& input, sendDataToSE_cb cb);
    ErrorCode finish(uint64_t operHandle, std::vector<uint8_t>& input, sendDataToSE_cb cb);

private:
    std::map<uint64_t, OperationData> operationTable;
    ErrorCode internalUpdate(uint64_t operHandle, uint8_t* input, size_t input_len, Operation opr, std::vector<uint8_t>&
    out);
    inline ErrorCode handleInternalUpdate(uint64_t operHandle, uint8_t* data, size_t len, Operation opr,
            sendDataToSE_cb cb) {
        ErrorCode errorCode = ErrorCode::OK;
        std::vector<uint8_t> out;
        if(ErrorCode::OK != (errorCode = internalUpdate(operHandle, data, len,
                        opr, out))) {
            return errorCode;
        }
        if(ErrorCode::OK != (errorCode = cb(out))) {
            return errorCode;
        }
        return errorCode;
    }


};

}  // namespace javacard
}  // namespace V4_1
}  // namespace keymaster

#endif  // KEYMASTER_V4_1_JAVACARD_OPERATIONCONTEXT_H_
