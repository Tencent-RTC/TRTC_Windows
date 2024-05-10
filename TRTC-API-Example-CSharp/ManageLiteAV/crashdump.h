/**************************************************************************
    Copyright:      Copyright ? 1998 - 2017 Tencent. All Rights Reserved
    Created:        2017-2-17 11:28:50
    Filename:       crashdump.h

    Description:
***************************************************************************/

#ifndef __CRASHDUMP_H__
#define __CRASHDUMP_H__

using namespace System;
using namespace System::Text;

/**************************************************************************/

namespace ManageLiteAV {

    public ref class CrashDump
    {
    public:
        explicit CrashDump();
        ~CrashDump();

        /// <summary>
        /// Enable minidump generation. For generate dump file to analyse crash cause.
        /// </summary>
        void open();

        /// <summary>
        /// Disable minidump generation
        /// </summary>
        void close();
    private:
        LPTOP_LEVEL_EXCEPTION_FILTER m_oldExceptionFilter;
    };

}

/**************************************************************************/
#endif /* __CRASHDUMP_H__ */
