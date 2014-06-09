﻿// ------------------------------------------------------------------------------
//  <auto-generated>
//      This code was generated by coded UI test builder.
//      Version: 11.0.0.0
//
//      Changes to this file may cause incorrect behavior and will be lost if
//      the code is regenerated.
//  </auto-generated>
// ------------------------------------------------------------------------------

namespace GUITest1
{
    using System;
    using System.CodeDom.Compiler;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Text.RegularExpressions;
    using System.Windows.Input;
    using Microsoft.VisualStudio.TestTools.UITest.Extension;
    using Microsoft.VisualStudio.TestTools.UITesting;
    using Microsoft.VisualStudio.TestTools.UITesting.WinControls;
    using Microsoft.VisualStudio.TestTools.UITesting.WpfControls;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using Keyboard = Microsoft.VisualStudio.TestTools.UITesting.Keyboard;
    using Mouse = Microsoft.VisualStudio.TestTools.UITesting.Mouse;
    using MouseButtons = System.Windows.Forms.MouseButtons;
    
    
    [GeneratedCode("Coded UITest Builder", "11.0.60315.1")]
    public partial class UIMap
    {
        
        /// <summary>
        /// SimulatronUITest - Use 'SimulatronUITestParams' to pass parameters into this method.
        /// </summary>
        public void SimulatronUITest()
        {
            #region Variable Declarations
            WinButton uIShowdesktopButton = this.UIItemWindow.UIShowdesktopButton;
            WpfCheckBox uICheckBoxCheckBox = this.UISimulatronWindowWindow.UICheckBoxCheckBox;
            WpfExpander uITemperatureExpander = this.UISimulatronWindowWindow.UITemperatureExpander;
            WpfButton uIResetButton = this.UISimulatronWindowWindow.UIResetButton;
            WinWindow uISimulatronWindowWindow1 = this.UISimulatronWindowWindow1;
            WpfTitleBar uISimulatronWindowTitleBar = this.UISimulatronWindowWindow.UISimulatronWindowTitleBar;
            #endregion

            // Click 'Show desktop' button
            Mouse.Click(uIShowdesktopButton, new Point(40, 12));

            // Launch 'D:\Media\Univer\Labs\Git\IDE\Lab#2\Lab2\Lab2\bin\Debug\Lab2.exe'
            ApplicationUnderTest uISimulatronWindowWindow = ApplicationUnderTest.Launch(this.SimulatronUITestParams.UISimulatronWindowWindowExePath, this.SimulatronUITestParams.UISimulatronWindowWindowAlternateExePath);

            // Select 'CheckBox' check box
            uICheckBoxCheckBox.Checked = this.SimulatronUITestParams.UICheckBoxCheckBoxChecked;

            // Expand 'Temperature' expander
            uITemperatureExpander.Expanded = this.SimulatronUITestParams.UITemperatureExpanderExpanded;

            // Click 'Reset' button
            Mouse.Click(uIResetButton, new Point(32, 16));

            // Double-Click 'SimulatronWindow' window
            Mouse.DoubleClick(uISimulatronWindowWindow1, new Point(1034, 343));

            // Click 'SimulatronWindow' title bar
            Mouse.Click(uISimulatronWindowTitleBar, new Point(1137, -1));
        }
        
        #region Properties
        public virtual SimulatronUITestParams SimulatronUITestParams
        {
            get
            {
                if ((this.mSimulatronUITestParams == null))
                {
                    this.mSimulatronUITestParams = new SimulatronUITestParams();
                }
                return this.mSimulatronUITestParams;
            }
        }
        
        public UIItemWindow UIItemWindow
        {
            get
            {
                if ((this.mUIItemWindow == null))
                {
                    this.mUIItemWindow = new UIItemWindow();
                }
                return this.mUIItemWindow;
            }
        }
        
        public UISimulatronWindowWindow UISimulatronWindowWindow
        {
            get
            {
                if ((this.mUISimulatronWindowWindow == null))
                {
                    this.mUISimulatronWindowWindow = new UISimulatronWindowWindow();
                }
                return this.mUISimulatronWindowWindow;
            }
        }
        
        public UISimulatronWindowWindow1 UISimulatronWindowWindow1
        {
            get
            {
                if ((this.mUISimulatronWindowWindow1 == null))
                {
                    this.mUISimulatronWindowWindow1 = new UISimulatronWindowWindow1();
                }
                return this.mUISimulatronWindowWindow1;
            }
        }
        #endregion
        
        #region Fields
        private SimulatronUITestParams mSimulatronUITestParams;
        
        private UIItemWindow mUIItemWindow;
        
        private UISimulatronWindowWindow mUISimulatronWindowWindow;
        
        private UISimulatronWindowWindow1 mUISimulatronWindowWindow1;
        #endregion
    }
    
    /// <summary>
    /// Parameters to be passed into 'SimulatronUITest'
    /// </summary>
    [GeneratedCode("Coded UITest Builder", "11.0.60315.1")]
    public class SimulatronUITestParams
    {
        
        #region Fields
        /// <summary>
        /// Launch 'D:\Media\Univer\Labs\Git\IDE\Lab#2\Lab2\Lab2\bin\Debug\Lab2.exe'
        /// </summary>
        public string UISimulatronWindowWindowExePath = "D:\\Media\\Univer\\Labs\\Git\\IDE\\Lab#2\\Lab2\\Lab2\\bin\\Debug\\Lab2.exe";
        
        /// <summary>
        /// Launch 'D:\Media\Univer\Labs\Git\IDE\Lab#2\Lab2\Lab2\bin\Debug\Lab2.exe'
        /// </summary>
        public string UISimulatronWindowWindowAlternateExePath = "D:\\Media\\Univer\\Labs\\Git\\IDE\\Lab#2\\Lab2\\Lab2\\bin\\Debug\\Lab2.exe";
        
        /// <summary>
        /// Select 'CheckBox' check box
        /// </summary>
        public bool UICheckBoxCheckBoxChecked = true;
        
        /// <summary>
        /// Expand 'Temperature' expander
        /// </summary>
        public bool UITemperatureExpanderExpanded = true;
        #endregion
    }
    
    [GeneratedCode("Coded UITest Builder", "11.0.60315.1")]
    public class UIItemWindow : WinWindow
    {
        
        public UIItemWindow()
        {
            #region Search Criteria
            this.SearchProperties[WinWindow.PropertyNames.AccessibleName] = "Show desktop";
            this.SearchProperties[WinWindow.PropertyNames.ClassName] = "TrayShowDesktopButtonWClass";
            #endregion
        }
        
        #region Properties
        public WinButton UIShowdesktopButton
        {
            get
            {
                if ((this.mUIShowdesktopButton == null))
                {
                    this.mUIShowdesktopButton = new WinButton(this);
                    #region Search Criteria
                    this.mUIShowdesktopButton.SearchProperties[WinButton.PropertyNames.Name] = "Show desktop";
                    #endregion
                }
                return this.mUIShowdesktopButton;
            }
        }
        #endregion
        
        #region Fields
        private WinButton mUIShowdesktopButton;
        #endregion
    }
    
    [GeneratedCode("Coded UITest Builder", "11.0.60315.1")]
    public class UISimulatronWindowWindow : WpfWindow
    {
        
        public UISimulatronWindowWindow()
        {
            #region Search Criteria
            this.SearchProperties[WpfWindow.PropertyNames.Name] = "SimulatronWindow";
            this.SearchProperties.Add(new PropertyExpression(WpfWindow.PropertyNames.ClassName, "HwndWrapper", PropertyExpressionOperator.Contains));
            this.WindowTitles.Add("SimulatronWindow");
            #endregion
        }
        
        #region Properties
        public WpfCheckBox UICheckBoxCheckBox
        {
            get
            {
                if ((this.mUICheckBoxCheckBox == null))
                {
                    this.mUICheckBoxCheckBox = new WpfCheckBox(this);
                    #region Search Criteria
                    this.mUICheckBoxCheckBox.SearchProperties[WpfCheckBox.PropertyNames.AutomationId] = "CheckBox1";
                    this.mUICheckBoxCheckBox.WindowTitles.Add("SimulatronWindow");
                    #endregion
                }
                return this.mUICheckBoxCheckBox;
            }
        }
        
        public WpfExpander UITemperatureExpander
        {
            get
            {
                if ((this.mUITemperatureExpander == null))
                {
                    this.mUITemperatureExpander = new WpfExpander(this);
                    #region Search Criteria
                    this.mUITemperatureExpander.SearchProperties[WpfExpander.PropertyNames.AutomationId] = "Expander1";
                    this.mUITemperatureExpander.WindowTitles.Add("SimulatronWindow");
                    #endregion
                }
                return this.mUITemperatureExpander;
            }
        }
        
        public WpfButton UIResetButton
        {
            get
            {
                if ((this.mUIResetButton == null))
                {
                    this.mUIResetButton = new WpfButton(this);
                    #region Search Criteria
                    this.mUIResetButton.SearchProperties[WpfButton.PropertyNames.AutomationId] = "ResetBtn";
                    this.mUIResetButton.WindowTitles.Add("SimulatronWindow");
                    #endregion
                }
                return this.mUIResetButton;
            }
        }
        
        public WpfTitleBar UISimulatronWindowTitleBar
        {
            get
            {
                if ((this.mUISimulatronWindowTitleBar == null))
                {
                    this.mUISimulatronWindowTitleBar = new WpfTitleBar(this);
                    #region Search Criteria
                    this.mUISimulatronWindowTitleBar.SearchProperties[WpfTitleBar.PropertyNames.AutomationId] = "TitleBar";
                    this.mUISimulatronWindowTitleBar.WindowTitles.Add("SimulatronWindow");
                    #endregion
                }
                return this.mUISimulatronWindowTitleBar;
            }
        }
        #endregion
        
        #region Fields
        private WpfCheckBox mUICheckBoxCheckBox;
        
        private WpfExpander mUITemperatureExpander;
        
        private WpfButton mUIResetButton;
        
        private WpfTitleBar mUISimulatronWindowTitleBar;
        #endregion
    }
    
    [GeneratedCode("Coded UITest Builder", "11.0.60315.1")]
    public class UISimulatronWindowWindow1 : WinWindow
    {
        
        public UISimulatronWindowWindow1()
        {
            #region Search Criteria
            this.SearchProperties[WinWindow.PropertyNames.Name] = "SimulatronWindow";
            this.SearchProperties.Add(new PropertyExpression(WinWindow.PropertyNames.ClassName, "HwndWrapper", PropertyExpressionOperator.Contains));
            this.WindowTitles.Add("SimulatronWindow");
            #endregion
        }
    }
}
