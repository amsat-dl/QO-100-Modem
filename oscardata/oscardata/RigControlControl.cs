using System;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace oscardata
{
    public partial class RigControlControl : UserControl
    {
        HamlibService hamlibService = new HamlibService();

        public RigControlControl()
        {
            InitializeComponent();
            frequencyLabel.Text = "";
        }

        private async void RigControlControl_Load(object sender, EventArgs e)
        {
            var installed = hamlibService.IsInstalled();
            tb_hamlibDetected.Text = installed ? $"{hamlibService.HamlibVersion} ({hamlibService.HamlibBitness})" : "not found";

            if (!installed)
            {
                tb_hamlibDetected.BackColor = Color.LightPink;
                return;
            }

            tb_hamlibDetected.BackColor = Color.LightGreen;

            var rigs = await hamlibService.GetSupportedRigs();

            combo_radio.Items.AddRange((rigs).OrderBy(r => r.Mfg).ThenBy(r => r.Model).ToArray());

            combo_radio.SelectedItem = rigs.SingleOrDefault(s => s.Number == hamlibService.HamlibRigNumber);
            tbRigDevice.Text = hamlibService.RigDevice;
        }

        private void btnTestPtt_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();
            hamlibService.AssertPtt();
        }

        private void btnReleasePtt_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();
            hamlibService.ReleasePtt();
        }

        private void SetHamlibOptions()
        {
            if (combo_radio.SelectedItem is HamlibRig rig && !string.IsNullOrWhiteSpace(tbRigDevice.Text))
            {
                hamlibService.SetRig(rig.Number, tbRigDevice.Text);
            }
            else
            {
                hamlibService.SetRig(0, null);
            }
        }

        private async void btnGetFrequency_Click(object sender, EventArgs e)
        {
            SetHamlibOptions();

            var f = await hamlibService.GetFrequency();

            frequencyLabel.Text = f == default ? "" : (f / 1000000.0).ToString("0.00000") + "MHz";
        }

        private void label4_DoubleClick(object sender, EventArgs e)
        {
            tbRigDevice.Text = "127.0.0.1:60002";
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            hamlibService.Save();
        }
    }
}