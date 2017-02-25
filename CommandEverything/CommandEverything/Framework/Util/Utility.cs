namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// A utility class.
    /// </summary>
    public static class Utility
    {
        /// <summary>
        /// Removes a word from a string.
        /// </summary>
        /// <param name="FullString"></param>
        /// <param name="WordToRemove"></param>
        /// <returns></returns>
        public static string RemoveWordFromString(string FullString, string WordToRemove)
        {
            int index = FullString.IndexOf(WordToRemove);
            int index2 = index + WordToRemove.Length;

            if (index != -1)
            {
                string Result = FullString.Remove(index, index2 - index);
                Result = Result.Trim();
                return Result;
            }

            return "Error";
        }

        /// <summary>
        /// Returns true if any of the words in DoesItContain are in DoIContain.
        /// </summary>
        /// <param name="DoIContain"></param>
        /// <param name="DoesItContain"></param>
        /// <returns></returns>
        public static bool DoesStringContain(string DoIContain, string[] DoesItContain)
        {
            foreach (string item in DoesItContain)
            {
                if (DoIContain.Contains(item))
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Logs the specified line to a text file.
        /// </summary>
        /// <param name="ToLog"></param>
        public static void Log(string ToLog)
        {

        }
    }
}