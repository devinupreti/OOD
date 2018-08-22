///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 2.3                                                           //
// Author : Devin Upreti, OOD Spring 2018                            //
// Source : Prof Jim Fawcett                                         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project 4.  It's 
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
 * ver 2.2 : 30 Apr 2018
 * - added DispatcherLoadBrowse and Connect
 * ver 2.1 : 28 Apr 2018
 * - added DispatcherLoadCheckIn
 * ver 2.0 : 22 Apr 2018
 * - added tabbed display
 * - moved remote file view to RemoteNavControl
 * - migrated some methods from MainWindow to RemoteNavControl
 * - added local file view
 * - added NoSqlDb with very small demo as server starts up
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
// - that can be fixed with a load failure event handler
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
using System.IO;
using MsgPassingCommunication;

namespace WpfApp1
{
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
      Console.Title = "Project4Demo GUI Console";
    }

    private Stack<string> pathStack_ = new Stack<string>();
    internal Translater translater;
    internal CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_ 
      = new Dictionary<string, Action<CsMessage>>();
    internal string saveFilesPath;
    internal string sendFilesPath;
    private bool testCin = true; // Test CheckIn once
    private bool testCout = true; // Test CheckOut once

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
        //ShowCheckInfo.Text += "Message Received \n";
        //ShowCheckInfo.Text += "---------------------- \n";
        //ShowCheckInfo.Text += "COMMAND : " + msg.value("command") + "\n";
        //ShowCheckInfo.Text += "TO : " + msg.value("to") + "\n";
        //ShowCheckInfo.Text += "FROM : " + msg.value("from") + "\n";
        //ShowCheckInfo.Text += "FILENAME : " + msg.value("fileinfo") + "\n" + "\n";
        statusBarText.Text = "CheckIn Successful for " + msg.value("fileinfo");
    }

    //----< load CheckIn processing into dispatcher dictionary >------
    private void DispatcherLoadCheckIn()
    {
        Action<CsMessage> getCheckIn = (CsMessage rcvMsg) =>
        { 
            //Console.Write("\n  processing incoming file");
            //string fileName = "";
            //var enumer = rcvMsg.attributes.GetEnumerator();
            //while (enumer.MoveNext())
            //{
            //    string key = enumer.Current.Key;
            //    if (key.Contains("sendingFile"))
            //    {
            //        fileName = enumer.Current.Value;
            //        break;
            //    }
            //}
            //if (fileName.Length > 0)
            //{
            //    Action<string> act = (string fileNm) =>
            //    {
            //        showFile(fileNm);
            //        string fromFilepath = saveFilesPath + "/" + fileName;
            //        fromFilepath = System.IO.Path.GetFullPath(fromFilepath);
            //        string checkoutpath = "../../../../LocalStorage/CheckOutFiles/" + fileNm;
            //        string fullcoPath = System.IO.Path.GetFullPath(checkoutpath);
            //        Console.Write("\n  fromFILEPATH \"{0}\"", fromFilepath);
            //        System.IO.File.Copy(fromFilepath, fullcoPath, true);
            //    };
            //    Dispatcher.Invoke(act, new object[] { fileName });
            //}    
            Action checkIn = () =>
            {
                showCheckMsg(rcvMsg);
            };
            Dispatcher.Invoke(checkIn, new Object[] { });
        };
        addClientProc("CheckIn", getCheckIn);
    }

    private void showBrowsemsg(CsMessage msg)
    {
        string keycount = msg.value("keycount");
        int kc = Int32.Parse(keycount);
        if(kc == 0) { ShowBrowse.Text = "No Keys in the Repository match your Query\n";  }
        else
        {
            ShowBrowse.Text = "------------------------------------------------------------\n";
            ShowBrowse.Text += "KEYS matching query in repository are :\n";
            for (int i = 0; i < kc; i++)
            {
                    string myString = "key" + (i + 1).ToString();
                    ShowBrowse.Text += msg.value(myString) + "\n";
            }
        }
        statusBarText.Text = "Browse Msg Recieved";
    }

    //----< load CheckIn processing into dispatcher dictionary >------
    private void DispatcherLoadBrowse()
    {
        Action<CsMessage> getBrowse = (CsMessage rcvMsg) =>
        {
            Action browse = () =>
            {
                showBrowsemsg(rcvMsg);
            };
            Dispatcher.Invoke(browse, new Object[] { });
        };
        addClientProc("Browse", getBrowse);
    }

    //----< process incoming messages on child thread >----------------
    private void processMessages()
    {
      ThreadStart thrdProc = () => {
        while (true)
        {
          CsMessage msg = translater.getMessage();
          try
          {
            string msgId = msg.value("command");
            Console.Write("\n  client getting message \"{0}\"", msgId);
            if (dispatcher_.ContainsKey(msgId))
              dispatcher_[msgId].Invoke(msg);
          }
          catch(Exception ex)
          {
            Console.Write("\n  {0}", ex.Message);
            msg.show();
          }
        }
      };
      rcvThrd = new Thread(thrdProc);
      rcvThrd.IsBackground = true;
      rcvThrd.Start();
    }
    //----< add client processing for message with key >---------------

    private void addClientProc(string key, Action<CsMessage> clientProc)
    {
      dispatcher_[key] = clientProc;
    }
    ////----< load getDirs processing into dispatcher dictionary >-------

    private void DispatcherLoadGetDirs()
    {
      Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
      {
        Action clrDirs = () =>
        {
          //NavLocal.clearDirs();
          NavRemote.clearDirs();
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
              NavRemote.addDir(dir);
            };
            Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
          }
        }
        Action insertUp = () =>
        {
          NavRemote.insertParent();
        };
        Dispatcher.Invoke(insertUp, new Object[] { });
      };
      addClientProc("getDirs", getDirs);
    }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
        Action clrFiles = () =>
        {
          NavRemote.clearFiles();
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
              NavRemote.addFile(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
          Action mytest = () =>
          {
              if (testCin == true)
              {
                  testCheckIn();
                  testBrowse();
                  testCin = false;
              }
              if (testCout == true)
              {
                  testCheckOut();
                  testConnect();
                  testCout = false;
              }
          };
          Dispatcher.Invoke(mytest, new Object[] { });
      };
      addClientProc("getFiles", getFiles);
    }

    //-------< Handles the reply msg recieved from Server with Command CheckOut>-----------
    private void showCheckOut(CsMessage msg)
    { 
        statusBarText.Text = "CheckOut Successful for " + msg.value("fileName");
    }

    //----< load getFiles processing into dispatcher dictionary >------
    private void DispatcherLoadSendFile()
    {
      Action<CsMessage> sendFile = (CsMessage rcvMsg) =>
      {
        Console.Write("\n  processing incoming file");
        string fileName = "";
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("sendingFile"))
          {
            fileName = enumer.Current.Value;
            break;
          }
        }
        if (fileName.Length > 0)
        {
          Action<string> act = (string fileNm) => {
              showFile(fileNm);
              string fromFilepath = saveFilesPath + "/" + fileName;
              fromFilepath = System.IO.Path.GetFullPath(fromFilepath);
              string checkoutpath = "../../../../LocalStorage/CheckOutFiles/" + fileNm;
              string fullcoPath = System.IO.Path.GetFullPath(checkoutpath);
              Console.Write("\n  fromFILEPATH \"{0}\"", fromFilepath);
              System.IO.File.Copy(fromFilepath, fullcoPath, true);
          };
          Dispatcher.Invoke(act, new object[] { fileName });
        }
        Action checkOut = () =>
        {
            showCheckOut(rcvMsg);
        };
        Dispatcher.Invoke(checkOut, new Object[] { });
      };
      addClientProc("sendFile", sendFile);
    }
    //----< load all dispatcher processing >---------------------------

    private void loadDispatcher()
    {
      DispatcherLoadGetDirs();
      DispatcherLoadGetFiles();
      DispatcherLoadSendFile();
      DispatcherLoadConnect();
      DispatcherLoadCheckIn();
      DispatcherLoadBrowse();
    }
    //----< start Comm, fill window display with dirs and files >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      // start Comm
      endPoint_ = new CsEndPoint(); 
      endPoint_.machineAddress = "localhost";
      endPoint_.port = 8082;
      NavRemote.navEndPoint_ = endPoint_;

      translater = new Translater();
      translater.listen(endPoint_);

      // start processing messages
      processMessages();

      // load dispatcher
      loadDispatcher();

      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      pathStack_.Push("../Storage");

      NavRemote.PathTextBlock.Text = "Storage";
      NavRemote.pathStack_.Push("../Storage");

      NavLocal.PathTextBlock.Text = "LocalStorage";
      NavLocal.pathStack_.Push("");
      NavLocal.localStorageRoot_ = "../../../../LocalStorage";
      saveFilesPath = translater.setSaveFilePath("../../../SaveFiles");
      sendFilesPath = translater.setSendFilePath("../../../SendFiles");

      NavLocal.refreshDisplay();
      NavRemote.refreshDisplay();
      //test1();
      //testConnect();
    }
    //----< strip off name of first part of path >---------------------

    public string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }
    //----< show file text >-------------------------------------------

    private void showFile(string fileName)
    {
      Paragraph paragraph = new Paragraph();
      string fileSpec = saveFilesPath + "\\" + fileName;
      string fileText = File.ReadAllText(fileSpec);
      paragraph.Inlines.Add(new Run(fileText));
      CodePopupWindow popUp = new CodePopupWindow();
      popUp.codeView.Blocks.Clear();
      popUp.codeView.Blocks.Add(paragraph);
      popUp.Show();
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

    //----< respond to mouse click on CheckOut Button >-----------------
    private void BrowseClick(object sender, RoutedEventArgs e)
    {
        string nameQ;
        nameQ = FileNametxtbox.Text;
        string desQ;
        desQ = Descriptiontxtbox.Text;
        string childQ;
        childQ = Childrentxtbox.Text;
        if (nameQ.Equals("") && desQ.Equals("") && childQ.Equals("")) { ShowBrowse.Text = "Missing Fields !!! \n\n"; }
        else
        {
            List<string> children = childQ.Split(',').ToList();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "Browse");
            msg.add("namequery", nameQ);
            msg.add("descquery", desQ);

            if (childQ != "")
            {
                for (int i = 0; i < children.Count; i++)
                {
                    string myString = "child" + (i + 1).ToString();
                    msg.add(myString, children[i]);
                }
            }
            translater.postMessage(msg);
        }
        }

        ////////////////////////////////////////////////////////////////////////////
        // TEST FUNCTIONS

        
        //----< test for CheckIn >---------------------------------
        void testCheckIn()
        {
            NavLocal.FileList.SelectedIndex = 2;
            NavLocal.ChkInDescriptiontxtbox.Text = "Test File";
            NavLocal.CheckInButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, NavLocal.CheckInButton));
        }

        //----< test for CheckOut >---------------------------------
        void testCheckOut()
        {
            NavRemote.FileList.SelectedIndex = 1;
            NavRemote.CheckOutButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, NavRemote.CheckOutButton));
        }

        //----< test for Browse >---------------------------------
        void testBrowse()
        {
            Descriptiontxtbox.Text = "Test File";
            BrowseButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, BrowseButton));
        }

        //----< test for Connect >---------------------------------
        void testConnect()
        {
            ConnectButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, ConnectButton));
        }

        //----< first test not completed >---------------------------------
        //void test1()
        //{
        //    MouseButtonEventArgs e = new MouseButtonEventArgs(null, 0, 0);
        //    NavRemote.DirList.SelectedIndex = 1;
        //    DirList.SelectedIndex = 1;
        //    DirList_MouseDoubleClck(this, e);
        //}
    }
}
