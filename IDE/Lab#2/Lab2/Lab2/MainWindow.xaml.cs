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
using System.Speech.Synthesis;
using System.Speech.AudioFormat;

namespace Lab2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {

        SpeechSynthesizer synthesizer = new SpeechSynthesizer();


        public MainWindow()
        {
            InitializeComponent();
            synthesizer.Volume = 100;  // 0...100
            synthesizer.Rate   = 0;     // -10...10
            synthesizer.SpeakAsync("The Simulatron has started!");
        }

        private void CheckBox1_Checked(object sender, RoutedEventArgs e)
        {
            synthesizer.SpeakAsync("Put your seat belts.");
        }

        private void ResetBtn_Click(object sender, RoutedEventArgs e)
        {
            Expander1.IsExpanded = false;
            CheckBox1.IsChecked = false;
            synthesizer.Volume = 100;
            synthesizer.Rate = 0;
            synthesizer.SpeakAsync("The system has been successfully reset.");
        }


    }
}
