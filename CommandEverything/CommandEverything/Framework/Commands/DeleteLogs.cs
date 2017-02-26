using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Deletes all of this program's logs.
    /// </summary>
    public class DeleteLogs : ICommand
    {
        public string GetHelp()
        {
            return "Deletes all logs created by all sessions of this program except for the currently running session.";
        }

        public string GetName()
        {
            return "Delete Logs";
        }

        public void Run(string Input)
        {
            Logging.DeleteLogs();
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "delete logs", "wipe logs", "delete log data", "wipe log data" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
