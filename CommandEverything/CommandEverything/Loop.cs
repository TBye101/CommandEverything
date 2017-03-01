using CommandEverything.Framework;
using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
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
        [STAThreadAttribute]
        public static void Main(string[] args)
        {
            Startup Init = new Startup();
            Init.Go();

            ConsoleWriter.WriteLine("Command Everything is initiating startup with " + args.Length + " arguments:");
            
            foreach (string item in args)
            {
                ConsoleWriter.WriteLine(item);
            }

            ConsoleWriter.WriteLine("Startup complete");

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