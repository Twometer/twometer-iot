using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace LumiSync
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
            Initialize();
        }

        public async void Initialize()
        {
            var controller = await this.ShowProgressAsync("Connecting...", "Please wait while LumiSync is contacting the IoT bridge...");
            controller.SetIndeterminate();
            controller.SetCancelable(true);

            controller.Canceled += (s, e) =>
            {
                controller.CloseAsync();
            };
        }
    }
}
