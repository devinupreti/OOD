///////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                          //
// ver 1.2                                                               //
// Author - Devin Upreti,, CSE687 - Object Oriented Design, Spring 2018  //
// Source : Prof. Jim Fawcett                                            //
///////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 : 7 Apr 2018
 *  - added support for tabs Browse, View File and View Metadata
 *  
 * ver 1.1 : 4 Apr 2018
 * - added support for tabs CheckIn, CheckOut and Connect
 * 
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
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
using System.Threading;
using MsgPassingCommunication; // has CommLibWrapper

namespace WpfApp1
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();
        private string info_;
        private bool testVF = true; // Test ViewFile once
        private bool testMD = true; // Test ViewMetadata once

        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        //----< function dispatched by child thread to main thread >-------
        private void clearDirs()
        {
            DirList.Items.Clear();
        }

        //----< function dispatched by child thread to main thread >-------
        private void addDir(string dir)
        {
            DirList.Items.Add(dir);
        }

        //----< function dispatched by child thread to main thread >-------
        private void insertParent()
        {
            DirList.Items.Insert(0, "..");
        }

        //----< function dispatched by child thread to main thread >-------
        private void clearFiles()
        {
            FileList.Items.Clear();
        }

        //----< function dispatched by child thread to main thread >-------
        private void addFile(string file)
        {
            FileList.Items.Add(file);
        }

        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< load getDirs processing into dispatcher dictionary >-------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
          {
                  clearDirs();
              };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                  {
                      addDir(dir);
                  };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
          {
                  insertParent();
              };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }
        
        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {  Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
              {
                  clearFiles();
              };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                  {
                      addFile(file);
                  };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
                Action mytest = () =>
                {
                    if (testMD == true)
                    {
                        testViewMeta();
                        testMD = false;
                    }
                    if (testVF == true)
                    {
                        testViewFile();
                        testVF = false;
                    }
                };
                Dispatcher.Invoke(mytest, new Object[] { });
            };
            addClientProc("getFiles", getFiles);
        }
        

        //-------< Handles the reply msg recieved from Server with Command Connect>-------------
        private void Connectme(CsMessage msg)
        {
            connecttxtbox.Text += "Connection Successful!!\n";
            connecttxtbox.Text += "Message Received \n";
            connecttxtbox.Text += "---------------------- \n";
            connecttxtbox.Text += "COMMAND : " + msg.value("command") + "\n";
            connecttxtbox.Text += "TO : " + msg.value("to") + "\n";
            connecttxtbox.Text += "FROM : " + msg.value("from") + "\n" + "\n";
            statusBarText.Text = "Recieved Connect Msg";
        }

        //----< load Connect processing into dispatcher dictionary >------
        private void DispatcherLoadConnect()
        {
            Action<CsMessage> getConnect = (CsMessage rcvMsg) =>
            {
                Action Connect = () =>
                {
                    Connectme(rcvMsg);
                };
                Dispatcher.Invoke(Connect, new Object[] { });
            };
            addClientProc("Connect", getConnect);
        }

        //-------< Handles the reply msg recieved from Server with Command CheckIn>-----------
        private void showCheckMsg(CsMessage msg)
        {
            ShowCheckInfo.Text += "Message Received \n";
            ShowCheckInfo.Text += "---------------------- \n";
            ShowCheckInfo.Text += "COMMAND : " + msg.value("command") + "\n";
            ShowCheckInfo.Text += "TO : " + msg.value("to") + "\n";
            ShowCheckInfo.Text += "FROM : " + msg.value("from") + "\n" ;
            ShowCheckInfo.Text += "FILENAME : " + msg.value("fileinfo") + "\n" + "\n";
            statusBarText.Text = "Recieved CheckIn Msg";
        }

        //----< load CheckIn processing into dispatcher dictionary >------
        private void DispatcherLoadCheckIn()
        {
            Action<CsMessage> getCheckIn = (CsMessage rcvMsg) =>
            {
                Action checkIn = () =>
                {
                    showCheckMsg(rcvMsg);
                };
                Dispatcher.Invoke(checkIn, new Object[] { });
            };
            addClientProc("CheckIn", getCheckIn);
        }

        //-------< Handles the reply msg recieved from Server with Command CheckOut>-----------
        private void showCheckOut(CsMessage msg)
        {
            ShowCheckOut.Text += "Message Received \n";
            ShowCheckOut.Text += "---------------------- \n";
            ShowCheckOut.Text += "COMMAND : " + msg.value("command") + "\n";
            ShowCheckOut.Text += "TO : " + msg.value("to") + "\n";
            ShowCheckOut.Text += "FROM : " + msg.value("from") + "\n";
            ShowCheckOut.Text += "FILENAME : " + msg.value("fileinfo") + "\n" + "\n";
            statusBarText.Text = "Recieved CheckOut Msg";
        }

        //----< load CheckOut processing into dispatcher dictionary >------
        private void DispatcherLoadCheckOut()
        {
            Action<CsMessage> getCheckOut = (CsMessage rcvMsg) =>
            {
                Action checkOut = () =>
                {
                    showCheckOut(rcvMsg);
                };
                Dispatcher.Invoke(checkOut, new Object[] { });
            };
            addClientProc("CheckOut", getCheckOut);
        }

        //-------< Handles the reply msg recieved from Server with Command ViewFile>-----------
        private void ViewFile(CsMessage msg)
        {
            ViewFiletxtbox.Text = "Message Received \n";
            ViewFiletxtbox.Text += "---------------------- \n";
            ViewFiletxtbox.Text += "COMMAND : " + msg.value("command") + "\n";
            ViewFiletxtbox.Text += "TO : " + msg.value("to") + "\n";
            ViewFiletxtbox.Text += "FROM : " + msg.value("from") + "\n";
            ViewFiletxtbox.Text += "PATH : " + msg.value("path") + "\n";
            ViewFiletxtbox.Text += "FILENAME : " + msg.value("fileinfo") + "\n" + "\n";
            string pathstring = "//" + msg.value("fileinfo");
            statusBarText.Text = "Recieved ViewFile Msg";
        }

        //----< load ViewFile processing into dispatcher dictionary >------
        private void DispatcherLoadViewFile()
        {
            Action<CsMessage> getViewFile = (CsMessage rcvMsg) =>
            {
                Action View = () =>
                {
                    ViewFile(rcvMsg);
                };
                Dispatcher.Invoke(View, new Object[] { });
            };
            addClientProc("ViewFile", getViewFile);
        }

        //-------< Handles the reply msg recieved from Server with Command ViewMetadata>-----------
        private void ViewMeta(CsMessage msg)
        {
            ViewFiletxtbox.Text = "Message Received \n";
            ViewFiletxtbox.Text += "---------------------- \n";
            ViewFiletxtbox.Text += "COMMAND : " + msg.value("command") + "\n";
            ViewFiletxtbox.Text += "TO : " + msg.value("to") + "\n";
            ViewFiletxtbox.Text += "FROM : " + msg.value("from") + "\n";
            ViewFiletxtbox.Text += "PACKAGE : " + msg.value("pkginfo") + "\n" + "\n";
            statusBarText.Text = "Recieved ViewMetadata Msg";
        }

        //----< load ViewMeatdata processing into dispatcher dictionary >------
        private void DispatcherLoadViewMetadata()
        {
            Action<CsMessage> getViewMeta = (CsMessage rcvMsg) =>
            {
                Action Meta = () =>
                {
                    ViewMeta(rcvMsg);
                };
                Dispatcher.Invoke(Meta, new Object[] { });
            };
            addClientProc("ViewMetadata", getViewMeta);
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadConnect();
            DispatcherLoadCheckIn();
            DispatcherLoadCheckOut();
            DispatcherLoadViewFile();
            DispatcherLoadViewMetadata();
        }

        //----< start Comm, fill ViewFile tab display with dirs and files >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // start Comm
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = 8082;
            translater = new Translater();
            translater.listen(endPoint_);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            PathTextBlock.Text = "Storage";
            pathStack_.Push("../Storage");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
            testConnect();
            testCheckIn();
            testCheckOut();
        }
        
        //----< strip off name of first part of path >---------------------
        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }
        
        //----< respond to mouse double-click on dir name >----------------
        private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            // build path for selected dir
            string selectedDir = (string)DirList.SelectedItem;
            string path;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Storage"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }
            // display path in Dir TextBlcok
            PathTextBlock.Text = removeFirstDir(pathStack_.Peek());

            // build message to get dirs and post it
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }

        //----< respond to mouse click on Connect Button >----------------
        private void ConnectButtonClick(object sender, RoutedEventArgs e)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Connect");
            connecttxtbox.Text += "Sending message to Server .... \n";
            translater.postMessage(msg);
        }

        //----< respond to mouse click on CheckIn Button >----------------
        private void CheckInClick(object sender, RoutedEventArgs e)
        {
            info_ = CheckIntxtbox.Text;
            if (info_ == "") { ShowCheckInfo.Text = "Missing Filename !!! \n\n"; }
            else
            {
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "CheckIn");
                msg.add("fileinfo", info_);
                translater.postMessage(msg);
            }
        }

        //----< respond to mouse click on CheckOut Button >-----------------
        private void CheckOutClick(object sender, RoutedEventArgs e)
        {

            info_ = CheckOuttxtbox.Text;
            if (info_ == "") { ShowCheckOut.Text = "Missing Filename !!! \n\n"; }
            else
            {
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;
                
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "CheckOut");
                msg.add("fileinfo", info_);
                translater.postMessage(msg);
            }
        }
        
        //----< respond to mouse click on ViewFile Button >----------------
        private void ViewFileClick(object sender, RoutedEventArgs e)
        {
            if (FileList.SelectedItem != null)
            {
                var fname = FileList.SelectedItem as string;

                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "ViewFile");
                msg.add("path", pathStack_.Peek() +"/"+ fname);
                msg.add("fileinfo", fname);
                translater.postMessage(msg);
            }
            else
            {
                ViewFiletxtbox.Text = "No File selected\n";
            }
        }

        //----< respond to mouse click on ViewMeta Button >----------------
        private void ViewMetaClick(object sender, RoutedEventArgs e)
        {
            if (DirList.SelectedItem != null)
            {
                var pkgname = DirList.SelectedItem as string;
                if (pkgname == "..") { pkgname = "Storage"; }
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "ViewMetadata");
                msg.add("pkginfo", pkgname);
                msg.add("path", pathStack_.Peek() + "/"); 
                translater.postMessage(msg);
            }
            else
            {
                ViewFiletxtbox.Text = "No Pkg selected\n";
            }
        }

        //----< test for Connect >---------------------------------
        void testConnect()
        {
            ConnectButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, ConnectButton));
        }

        //----< test for CheckIn >---------------------------------
        void testCheckIn()
        {
            CheckIntxtbox.Text = "pkg1.h";
            CheckInButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, CheckInButton));
        }

        //----< test for CheckOut >---------------------------------
        void testCheckOut()
        {
            CheckOuttxtbox.Text = "pkg2.h";
            CheckOutButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, CheckOutButton));
        }
        
        //----< test for ViewFile >---------------------------------
        void testViewFile()
        {
            FileList.SelectedIndex = 2;
            ViewFileButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, ViewFileButton));
        }

        //----< test for ViewMetadata >---------------------------------
        void testViewMeta()
        {
            DirList.SelectedIndex = 1;
            ViewMetaButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, ViewMetaButton));
        }

    }
}
