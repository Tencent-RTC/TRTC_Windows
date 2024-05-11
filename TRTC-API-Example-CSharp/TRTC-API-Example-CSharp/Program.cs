using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using TRTCCSharpDemo.Common;
using System.Diagnostics;
using System.Threading;

namespace TRTCCSharpDemo
{
    static class Program
    {
        // External function declaration
        [DllImport("User32.dll")]
        private static extern Int32 SetProcessDPIAware();

        public static EventWaitHandle ProgramStarted;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            ManageLiteAV.CrashDump dump = new ManageLiteAV.CrashDump();
            dump.open();

            // Try to create a named event
            bool createNew;
            ProgramStarted = new EventWaitHandle(false, EventResetMode.AutoReset, "TRTCStartEvent", out createNew);

            // If the named event already exists (there is a previous running instance),
            // it is notified of the event and exits
            if (!createNew)
            {
                ProgramStarted.Set();
                return;
            }

            SetProcessDPIAware();   // High DPI is disabled by default to prevent SDK recording errors

            Log.Open();
            // Initialize the Local configuration of the SDK
            DataManager.GetInstance().InitConfig();

            Process processes = Process.GetCurrentProcess();
            Log.I(String.Format("Progress <{0}, {1}>", processes.ProcessName, processes.Id));

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new TRTCCSharpDemo.PortalWindow());

            // Write the latest Local configuration information before exiting the program.
            DataManager.GetInstance().Uninit();
            DataManager.GetInstance().Dispose();

            Log.Close();

            dump.close();
        }
    }
}
