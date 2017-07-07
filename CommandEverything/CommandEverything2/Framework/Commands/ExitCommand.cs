using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Exits the current program.
    /// </summary>
    public class ExitCommand : ICommand
    {
        /// <summary>
        /// Gets help information about this command.
        /// </summary>
        /// <returns></returns>
        public string GetHelp()
        {
            return "Used to exit the current console.";
        }

        /// <summary>
        /// Returns the name of this command.
        /// </summary>
        /// <returns></returns>
        public string GetName()
        {
            return "Exit";
        }

        /// <summary>
        /// Runs this command.
        /// </summary>
        /// <param name="Input"></param>
        public void Run(string Input)
        {
            Environment.Exit(0);
        }

        /// <summary>
        /// Decides if the input means we should run this command.
        /// </summary>
        /// <param name="Input"></param>
        /// <returns></returns>
        public bool ShouldRunThisCommand(string Input)
        {
            string[] ValidInput = { "quit", "exit" };

            foreach (string item in ValidInput)
            {
                if (item == Input.ToLower())
                {
                    return true;
                }
            }

            return false;
        }
    }
}