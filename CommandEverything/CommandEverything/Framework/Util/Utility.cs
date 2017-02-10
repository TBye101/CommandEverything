﻿namespace CommandEverything.Framework.Util
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
            int index2 = FullString.LastIndexOf(WordToRemove);

            if (index != -1)
            {
                string Result = FullString.Remove(index, index2 - index);
                return Result;
            }

            return "Error";
        }
    }
}