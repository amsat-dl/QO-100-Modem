using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using static oscardata.Command;

namespace oscardata
{
    internal class HamlibService
    {
        private string rigctlFile;
        private string appdataPath;
        private string settingsFile => Path.Combine(appdataPath, "hamlibsettings.json");

        public HamlibService()
        {
            var programFiles = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            var hamlibFolder = Path.Combine(programFiles, "hamlib-w64-4.5.5", "bin");
            rigctlFile = Path.Combine(hamlibFolder, "rigctl.exe");

            var path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            appdataPath = Path.Combine(path, "oscardata");
            if (!Directory.Exists(appdataPath))
            {
                Directory.CreateDirectory(appdataPath);
            }

            if (File.Exists(settingsFile))
            {
                var settings = JsonConvert.DeserializeObject<HamlibSettings>(File.ReadAllText(settingsFile));
                HamlibRigNumber = settings.RigNumber;
                RigDevice = settings.Device;
            }
        }

        internal bool IsInstalled()
        {
            return File.Exists(rigctlFile);
        }

        public async Task<ICollection<HamlibRig>> GetSupportedRigs()
        {
            var (stdout, _) = await ReadAsync(rigctlFile, "-l");

            var rigs = new List<HamlibRig>();

            foreach (var line in stdout.Split('\r', '\n').Skip(1))
            {
                if (string.IsNullOrWhiteSpace(line)) continue;
                var number = line.Substring(0, 6);
                var mfg = line.Substring(8, 23);
                var model = line.Substring(31, 24);
                var version = line.Substring(55, 16);
                var status = line.Substring(71, 12);
                var macro = line.Substring(83);

                var rig = new HamlibRig
                {
                    Number = int.Parse(number),
                    Mfg = mfg.Trim(),
                    Model = model.Trim(),
                    Version = version.Trim(),
                    Status = status.Trim(),
                    Macro = macro.Trim()
                };

                rigs.Add(rig);
            }

            return rigs;
        }

        public int HamlibRigNumber { get; private set; }
        public string RigDevice { get; private set; }

        public void SetRig(int hamlibNumber, string rigDevice)
        {
            HamlibRigNumber = hamlibNumber;
            this.RigDevice = rigDevice;
        }

        public void Save()
        {
            var json = JsonConvert.SerializeObject(new HamlibSettings { Device = RigDevice, RigNumber = HamlibRigNumber });
            File.WriteAllText(settingsFile, json);
        }

        private class HamlibSettings
        {
            public int RigNumber { get; set; }
            public string Device { get; set; }
        }

        internal async Task<long> GetFrequency()
        {
            var (stdOut, _) = await ReadAsync(rigctlFile, $"{GetArgs()} f");
            if (string.IsNullOrWhiteSpace(stdOut))
            {
                return 0;
            }
            return long.Parse(stdOut.Trim());
        }

        private string GetArgs()
        {
            return $"-m {HamlibRigNumber} -r {RigDevice}";
        }

        internal async void AssertPtt()
        {
            await ReadAsync(rigctlFile, $"{GetArgs()} T 1");
        }

        internal async void ReleasePtt()
        {
            await ReadAsync(rigctlFile, $"{GetArgs()} T 0");
        }
    }

    internal class HamlibRig
    {
        public int Number { get; set; }
        public string Mfg { get; set; }
        public string Model { get; set; }
        public string Version { get; set; }
        public string Status { get; set; }
        public string Macro { get; set; }

        public override string ToString() => $"{Mfg} {Model}";
    }
}
