using CommandEverything.Framework;
using CommandEverything.Framework.Util;
using System;

namespace CommandEverything
{
    /// <summary>
    /// The main loop for this command line program.
    /// </summary>
    public class Loop
    {
        /// <summary>
        /// The entry point for this program.
        /// </summary>
        /// <param name="args"></param>
        public static void Main(string[] args)
        {
            CommandInterpreter.StartUp();
            while (true)
            {
                try
                {
                    CommandInterpreter.RecieveInput(Console.ReadLine());
                }
                catch (Exception TheException)
                {
                    Error.Report(TheException);
                }
            }
        }
    }
}