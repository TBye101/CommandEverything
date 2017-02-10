using CommandEverything.Framework;
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
                CommandInterpreter.RecieveInput(Console.ReadLine());
            }
        }
    }
}