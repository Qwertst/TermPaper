#include "include/WSEML.h"
#include "include/parser.h"
#include <iostream>


int main(){
    WSEML stck = parse("{"
                     "1:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_tmpPtr]ref, RD:$, D:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:stck]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:info]ps, 5:$[t:k, k:wfrm]ps, 6:$[t:i, i:0]ps]ptr]ref, I:$, N:$]bc, "
                     "2:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_helpPtr]ref, RD:$, D:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:stck]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:info]ps, 5:$[t:k, k:wfrm]ps, 6:$[t:i, i:0]ps]ptr]ref, I:$, N:$]bc, "
                     "3:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_cmdCopy]ref, RD:$, D:$, I:$, N:$]bc, "
                     "4:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_curStack]ref, RD:$, D:$, I:$, N:$]bc, "
                     "5:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_curFrm]ref, RD:$, D:$, I:$, N:$]bc, "
                     "6:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_newStackKey]ref, RD:$, D:$, I:$, N:$]bc, "
                     "7:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_nextFlag]ref, RD:$, D:$, I:$, N:$]bc, "
                     "8:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_bool]ref, RD:$, D:$, I:$, N:$]bc, "
                     "9:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_R]ref, RD:$, D:$, I:$, N:$]bc, "
                     "10:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps]ptr]ref, RK:$, K:$[type:i, 1:interrupt_O]ref, RD:$, D:$, I:$, N:$]bc, "
                     "11:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:stck]ps, 3:$[t:k, k:info]ps, 4:$[t:k, k:wlist]ps, 5:$[t:i, i:0]ps]ptr]ref, N:$]bc,"
                     "12:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "13:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref]ref, N:$]bc, "
                     "14:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref, data:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:stck]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:$]ps, 5:$[t:k, k:ip]ps]ptr]ref, N:$]bc,"
                     "15:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "16:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:4]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, N:$]bc,"
                     "17:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps]ptr]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, N:$]bc,"
                     "18:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_R]ps]ptr]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps, 4:$[t:k, k:R]ps]ptr]ref]ref, N:$]bc,"
                     "19:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_O]ps]ptr]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps, 4:$[t:k, k:O]ps]ptr]ref]ref, N:$]bc,"
                     "20:$[type:`=', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_nextFlag]ps]ptr]ref, O1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps, 4:$[t:k, k:N]ps]ptr]ref, O2:$[type:i, i:$]ref, N:$]bc, "
                     "21:$[type:`?', C:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_nextFlag]ps]ptr]ref, T:$[t:k, k:24]ps, F:$[t:k, k:22]ps]st,"
                     "22:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref, RK:$, K:$[type:i, i:6]ref, RD:$, D:$[type:i, 1:$[t:i, i:-1]ps]ref, I:$, N:$]bc, "
                     "23:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps, 4:$[t:k, k:N]ps]ptr]ref, N:$]bc,"
                     "24:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref, data:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:$]ps]ptr]ref, N:$]bc,"
                     "25:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "26:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:4]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, N:$]bc,"
                     "27:$[type:`C', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_bool]ps]ptr]ref, F:$[dllName:WSEML_DLL, funcName:isKeyExists]func, A:{list:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, key:$[type:i, 1:resume]ref}, N:$]bc, "
                     "28:$[type:`?', C:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_bool]ps]ptr]ref, T:$[t:k, k:29]ps, F:$[t:k, k:35]ps]st,"
                     "29:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref, RK:$, K:$, RD:$, D:$[type:i, 1:$[t:k, k:resume]ps]ref, I:$, N:$]bc, "
                     "30:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref, RK:$, K:$, RD:$, D:$[type:i, 1:$[t:k, k:R]ps]ref, I:$, N:$]bc, "
                     "31:$[type:`:=', dest:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_O]ps]ptr]ref, N:$]bc,"
                     "32:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:i,i:-1]ps]ptr]ref, N:$]bc, "
                     "33:$[type:`D', R:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, N:$]bc, "
                     "34:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:i,i:-1]ps]ptr]ref, N:$]bc, "
                     "35:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref, data:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:$]ps, 5:$[t:k, k:prev]ps, 6:$[t:i, i:0]ps]ptr]ref, N:$]bc,"
                     "36:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "37:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps, 4:$[t:k, k:4]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, N:$]bc,"
                     "38:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:4]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref]ref, N:$]bc,"
                     "39:$[type:`I', R:$, L:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref]ref, RK:$, K:$[type:i, i:interrupt]ref, RD:$, D:$[type:i, 1:{}]ref, I:$, N:$]bc, "
                     "40:$[type:`I', R:$, L:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref, RK:$, K:$[type:i, i:5]ref, RD:$, D:$[type:i, 1:$[t:k, k:interrupt]ps]ref, I:$, N:$]bc, "
                     "41:$[type:`I', R:$, L:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref]ref, RK:$, K:$[type:i, i:R]ref, RD:$, D:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_R]ps]ptr]ref, I:$, N:$]bc, "
                     "42:$[type:`I', R:$, L:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k,k:interrupt_tmpPtr]ps]ptr]ref]ref, RK:$, K:$[type:i, i:O]ref, RD:$, D:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_O]ps]ptr]ref, I:$, N:$]bc, "
                     "43:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:5]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:i, 1:next]ptr]ref, N:$]bc,"
                     "44:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref, data:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:stck]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:info]ps, 5:$[t:k, k:wfrm]ps, 6:$[t:i, i:0]ps]ptr]ref, N:$]bc,"
                     "45:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "46:$[type:`C', R:$, F:$[dllName:WSEML_DLL, funcName:insertPair]func, A:{list:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref]ref, I:$}, N:$]bc, "
                     "47:$[type:`D', R:$[type:d: 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref]ref, N:$]bc, "
                     "48:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref, data:$[type:i, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:$]ps, 4:$[t:k, k:$]ps, 5:$[t:k, k:prev]ps, 6:$[t:i, i:0]ps]ptr]ref, N:$]bc,"
                     "49:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:3]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc,"
                     "50:$[type:`:=', dest:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps, 4:$[t:k, k:4]ps, 5:$[t:k, k:k]ps]ptr]ref, data:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, N:$]bc,"
                     "51:$[type:`C', R:$, F:$[dllName:WSEML_DLL, funcName:insertPair]func, A:{list:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref]ref, data:$[type:d, 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, I:$[type:i, 1:0]ref}, N:$]bc, "
                     "52:$[type:`D', R:$[type:d: 1:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref]ref, N:$]bc, "
                     "53:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_tmpPtr]ps]ptr]ref, N:$]bc, "
                     "54:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_helpPtr]ps]ptr]ref, N:$]bc, "
                     "55:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_cmdCopy]ps]ptr]ref, N:$]bc, "
                     "56:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curStack]ps]ptr]ref, N:$]bc, "
                     "57:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_curFrm]ps]ptr]ref, N:$]bc, "
                     "58:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_newStackKey]ps]ptr]ref, N:$]bc, "
                     "59:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_nextFlag]ps]ptr]ref, N:$]bc"
                     "60:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_O]ps]ptr]ref, N:$]bc"
                     "61:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_R]ps]ptr]ref, N:$]bc"
                     "62:$[type:`D', R:$[type:d, 1:$[1:$[t:r]ps, 2:$[t:k, k:data]ps, 3:$[t:k, k:interrupt_bool]ps]ptr]ref, N:$]bc"
                     "}");
    std::string s = pack(stck);
    std::cout << s;
    return 0;
}