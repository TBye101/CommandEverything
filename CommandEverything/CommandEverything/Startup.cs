using CommandEverything.Framework.Util;
using System;
using System.IO;

namespace CommandEverything
{
    /// <summary>
    /// Does some startup logic.
    /// </summary>
    public class Startup
    {
        /// <summary>
        /// Runs some startup logic.
        /// </summary>
        public void Go()
        {

        }

        /// <summary>
        /// Verifies that a folder exists. If it doesn't, it is created.
        /// </summary>
        /// <param name="Path"></param>
        public void VerifyFolder(string Path)
        {
            try
            {
                if (!Directory.Exists(Path))
                {
                    Directory.CreateDirectory(Path);
                }
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }
    }
}
