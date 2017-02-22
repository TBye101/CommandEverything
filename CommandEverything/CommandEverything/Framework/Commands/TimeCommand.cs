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
    /// Displays the time.
    /// </summary>
    public class TimeCommand : ICommand
    {
        /// <summary>
        /// Returns help information for this command.
        /// </summary>
        /// <returns></returns>
        public string GetHelp()
        {
            return "Returns the system time.";
        }

        /// <summary>
        /// Returns the name of this command.
        /// </summary>
        /// <returns></returns>
        public string GetName()
        {
            return "Time";
        }

        /// <summary>
        /// Runs this command.
        /// </summary>
        /// <param name="Input"></param>
        public void Run(string Input)
        {
            ConsoleWriter.WriteLine(DateTime.UtcNow.ToString());
        }

        /// <summary>
        /// Returns if this command should be run based off of the specified input.
        /// </summary>
        /// <param name="Input"></param>
        /// <returns></returns>
        public bool ShouldRunThisCommand(string Input)
        {
            return Input.Contains("time");
        }
    }
}