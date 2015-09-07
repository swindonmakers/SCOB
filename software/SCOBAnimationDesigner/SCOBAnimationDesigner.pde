import processing.serial.*;
import controlP5.*;
import java.util.Arrays;
import java.io.BufferedWriter;
import java.io.FileWriter;

ControlP5 cp5;
PFont f;

Button connectBut;
Slider progSlider;

Slider LeftAnkle, LeftHip, RightHip, RightAnkle, ToDur;

ListBox anim;

DropdownList dlSerialPorts;
StringList serialLog;
Serial port;

JSONObject config;

float x=0, y=0, z=0;

String lastCmd = "";
String lastCmdRaw = "";
int lastLineNo = 0;
int resendCount = 0;

StringList sendBuffer = new StringList();
StringList comments = new StringList();
StringList loadedLines = new StringList();

boolean cts = true;
boolean resend = false;

PFont pf = createFont("Arial", 15, true);

int selectedStep = -1;

void saveConfig() {
  // save connection string
  saveJSONObject(config, "config.json");
}

void stop() {
  saveConfig();
  Disconnect(0);

  super.stop();
}


void setup() {
  size(800, 600, P3D);
  f = createFont("Arial", 15, true);

  try {
    config = loadJSONObject("config.json");
  }
  catch(Exception e) {
  }

  cp5 = new ControlP5(this);
  cp5.setControlFont(pf);

  // Connect button
  connectBut = cp5.addButton("Connect")
    .setPosition(0, 30);
  styleButton("Connect",140,40);
  //connectBut.setColorBackground(color(255,0,0,120));

  // Disconnect button
  Button disconnectBut = cp5.addButton("Disconnect")
    .setPosition(150, 30);
  styleButton("Disconnect",140,40);
  //connectBut.setColorBackground(color(255,0,0,120));
  
  
  Button FDBut = cp5.addButton("FD")
    .setPosition(0, 80);
  styleButton("FD",65,40);
  
  Button BKBut = cp5.addButton("BK")
    .setPosition(75, 80);
  styleButton("BK",65,40);
  
  Button LTBut = cp5.addButton("LT")
    .setPosition(150, 80);
  styleButton("LT",65,40);
  
  Button RTBut = cp5.addButton("RT")
    .setPosition(225, 80);
  styleButton("RT",65,40);
  
  Button STBut = cp5.addButton("ST")
    .setPosition(0, 130);
  styleButton("ST",65,40);


  Textfield newCommand = cp5.addTextfield("NewCommand")
    .setPosition(0, 200)
    .setAutoClear(false);
   styleTextfield("NewCommand", 290, 30);

  cp5.addButton("LoadCmds")
    .setPosition(0, 250);
  styleButton("LoadCmds",140,40);

  cp5.addButton("PlayBack")
    .setPosition(150, 250);
  styleButton("PlayBack",140,40);

  progSlider = cp5.addSlider("PlayBackProgress")
     .lock()
     .setPosition(0,300)
     .setRange(0,1)
     ;
  styleSlider("PlayBackProgress", 290, 20);
  progSlider.getCaptionLabel().setVisible(false);
  
  
  // Joint sliders
  
  LeftHip = cp5.addSlider("LeftHip")
     .setPosition(340,50)
     .setValue(0)
     ;
  styleJointSlider("LeftHip", 200, 20);
  
  RightHip = cp5.addSlider("RightHip")
     .setPosition(560,50)
     .setValue(0)
     ;
  styleJointSlider("RightHip", 200, 20);
  
  LeftAnkle = cp5.addSlider("LeftAnkle")
     .setPosition(340,100)
     .setValue(0)
     ;
  styleJointSlider("LeftAnkle", 200, 20);
  
  RightAnkle = cp5.addSlider("RightAnkle")
     .setPosition(560,100)
     .setValue(0)
     ;
  styleJointSlider("RightAnkle", 200, 20);
  
  ToDur = cp5.addSlider("ToDur")
     .setPosition(340,150)
     ;
  styleJointSlider("ToDur", 200, 20);
  ToDur.setRange(100,5000)
       .setValue(1000)
       .setNumberOfTickMarks(50)
       .showTickMarks(true)
       .getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE);
       
  // To button
  cp5.addButton("TO")
    .setPosition(560, 130);
  styleButton("TO",200,40);
  
  cp5.addButton("ResetJoints")
    .setPosition(340,180);
  styleButton("ResetJoints",90,40);
  
  cp5.addButton("AddToAnim")
    .setPosition(560, 180);
  styleButton("AddToAnim",200,40);

  anim = cp5.addListBox("Animation")
         .setPosition(340, 260);
  styleListbox(anim, 420, 225);

  cp5.addButton("PlayAnim")
    .setPosition(340, 480);
  styleButton("PlayAnim",90,40);
  
  cp5.addButton("ClearAnim")
    .setPosition(440, 480);
  styleButton("ClearAnim",90,40);
  
  cp5.addButton("RemoveStep")
    .setPosition(540, 480);
  styleButton("RemoveStep",90,40);
  
  Button promoteStep = cp5.addButton("PromoteStep")
    .setPosition(640, 480);
  styleButton("PromoteStep",50,40);
  promoteStep.captionLabel().set(str(char(8657)));
  
  Button demoteStep = cp5.addButton("DemoteStep")
    .setPosition(700, 480);
  styleButton("DemoteStep",50,40);
  demoteStep.captionLabel().set(str(char(8659)));

  // Configure Serial Ports Dropdown
  // -------------------------------
  dlSerialPorts = cp5.addDropdownList("dlSerialPorts")
    .setPosition(0, 25)
      ;
  dlSerialPorts.captionLabel().set("Choose a Serial Port");
  styleDropdownList(dlSerialPorts, 290, 25);
  // Get list of ports
  dlSerialPorts.addItems(Serial.list());

  if (config != null) {
    int i = Arrays.asList(Serial.list()).indexOf(config.getString("port"));
    dlSerialPorts.setValue(i);
  }

  // Configure serialLog
  serialLog = new StringList();
  serialLog.append("Log");
  serialLog.append("---");
}

public void Connect(int theValue) {
  if (dlSerialPorts.getValue() >= 0) {
    String c = Serial.list()[int(dlSerialPorts.getValue())];
    port = new Serial(this, c, 9600);
    port.bufferUntil(10);  // line feed

    config.setString("port", c);
    saveConfig();
    
    connectBut.setColorBackground(color(0,150,0,255));
  }
}

public void Disconnect(int theValue) {
  if (port != null) {
    port.dispose();
    port = null;

    sendBuffer.clear();
    
    connectBut.setColorBackground(color(0,0,0,255));
  }
}

public void SendCMD(String c) {
  if (port != null) {
    serialLog.append(c + "\r\n");
    //appendTextToFile("serial.log",c + "\r\n");
    cts = false; // reset by receiving an ok
    println("Sending: "+c);
    port.write(c + "\r\n");
    lastCmd = c;
    //cp5.get(Textfield.class, "LastCommand").setText(c);
  }
}

public void QueueCMD(String c) {
  println("Queued: "+c); 
  sendBuffer.append(c);
   
   int ns = sendBuffer.size();
   
   if (ns > progSlider.getMax())
     progSlider.setRange(0, ns);
}


public void SendCommand(int v) {
  QueueCMD(cp5.get(Textfield.class, "NewCommand").getText());
}

public void NewCommand(String s) {
  QueueCMD(cp5.get(Textfield.class, "NewCommand").getText());
}


public void LoadCmds(int v) {
  selectInput("Select a command file:", "fileSelected");
}

public void fileSelected(File selection) {
  if (selection == null) return;
  
  
  String lines[] = loadStrings(selection.getAbsolutePath()); 
  
  // parse out comments
  loadedLines.clear();
  comments.clear();
  
  for (int i = 0 ; i < lines.length; i++) {
     if (lines[i] != "") {
        if (lines[i].charAt(0) == ';') {
           comments.append(lines[i].substring(1));
        } else {
           loadedLines.append(lines[i]);
        } 
     }
  }
  
  comments.append("");
  comments.append(loadedLines.size() + " lines");
}

public void PlayBack(int v) {
  for (int i = 0 ; i < loadedLines.size(); i++) {
     if (loadedLines.get(i) != "") {
        QueueCMD(loadedLines.get(i));
     }
  }
}

void delay(int delay)
{
  int time = millis();
  while(millis() - time <= delay);
}

void serialEvent(Serial p) {
  String s = p.readString();
  
  // check for resend
  if (s.length() >= 4 && s.substring(0,4).equals("BUSY")) {
     resend = true;
  }
  
  if (s.length() > 1 && s.substring(0,2).equals("OK")) cts = true;
  serialLog.append(s);
  //appendTextToFile("serial.log",s);
  while (serialLog.size () > 16) serialLog.remove(0);
}


void keyPressed() {
  if (key=='1') {
  }
}


void draw() {
  background(255);
  textFont(f, 15);
  
  // draw background for Animation listbox - crappy hack
  fill(240);
  stroke(255);
  rect(340,260,420,205);
  
  
try {
  // Draw serialLog
  fill(0);
  for (int i=0; i<serialLog.size (); i++) {
    String s = serialLog.get(i);
    text(s, 10, 350 + i*16);
  }

  // send cmds
  if (resend) {
    SendCMD(lastCmdRaw);
    resend = false;
    resendCount++;
  } else if (sendBuffer.size() > 0 && cts) {
    String c = sendBuffer.get(0);
    sendBuffer.remove(0);
    lastLineNo += 1;
    lastCmdRaw = c;
    SendCMD(c);
  }
  
  progSlider.setValue(sendBuffer.size());
  
} catch(Exception e) {
  e.printStackTrace();
}
}


/**
 * Appends text to the end of a text file located in the data directory,
 * creates the file if it does not exist.
 * Can be used for big files with lots of rows,
 * existing lines will not be rewritten
 */
void appendTextToFile(String filename, String text){
  File f = new File(dataPath(filename));
  if(!f.exists()){
    createFile(f);
  }
  try {
    PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(f, true)));
    out.print(text);
    out.close();
  }catch (Exception e){
      e.printStackTrace();
  }
}

/**
 * Creates a new file including all subfolders
 */
void createFile(File f){
  File parentDir = f.getParentFile();
  try{
    parentDir.mkdirs();
    f.createNewFile();
  }catch(Exception e){
    e.printStackTrace();
  }
}


void styleButton(String s, int w, int h) {
   Controller c = cp5.getController(s);
  
   if (c != null) {
     c.setSize(w,h);
     
     Label l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     l.align(CENTER, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(150,150,150, 255));
     c.setColorForeground(color(100,100,100, 255));
     
   }
}


void styleDropdownList(DropdownList c, int w, int h) {
  
   if (c != null) {
     c.setSize(w, 150);
     c.setBarHeight(h);
     c.setItemHeight(25);
     c.toUpperCase(false);
     
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     //l.align(LEFT, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(255,0,0, 180));
     c.setColorForeground(color(255,0,0, 255));
   }
}

void styleListbox(ListBox c, int w, int h) {
  
   if (c != null) {
     c.setSize(w, h);
     c.setBarHeight(25);
     c.setItemHeight(25);
     c.toUpperCase(false);
     c.disableCollapse();
     
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     //l.align(LEFT, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(255,0,0, 180));
     c.setColorForeground(color(255,0,0, 255));
   }
}

void styleSlider(String s, int w, int h) {
   Controller c = cp5.getController(s);
  
   if (c != null) {
     c.setSize(w,h);
     
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     //l.align(LEFT, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(255,0,0, 255));
     c.setColorForeground(color(255,0,0, 255));
     
   }
}

void styleJointSlider(String s, int w, int h) {
   Slider c = (Slider)cp5.getController(s);
  
   if (c != null) {
     c.setSize(w,h);
     
     c.setRange(-90,90);
     c.setHandleSize(40);
     //c.setNumberOfTickMarks(181);
     c.setSliderMode(Slider.FLEXIBLE);
     c.setDecimalPrecision(0);
     
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     l.align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(255,0,0, 255));
     c.setColorForeground(color(255,0,0, 255));
     c.setColorCaptionLabel(color(0));
     
     
  
   }
}

void styleTextlabel(String s) {
   Controller c = cp5.getController(s);
  
   if (c != null) {
     c.setHeight(25);
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     //l.align(LEFT, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(0,0,0, 255));
     c.setColorForeground(color(0,0,0, 255));
     
   }
}

void styleTextfield(String s, int w, int h) {
   Controller c = cp5.getController(s);
  
   if (c != null) {
     c.setSize(w,h);
     Label l = c.getValueLabel();
     l.toUpperCase(false);
     l.setFont(pf);
      
     l = c.getCaptionLabel();
     l.toUpperCase(false);
     l.setFont(pf);
     l.align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(0,0,0, 255));
     c.setColorForeground(color(0,0,0, 255));
     c.setColorCaptionLabel(color(0));
     
   }
}


public void FD(int v) {
  QueueCMD("FD");
}

public void BK(int v) {
  QueueCMD("BK");
}

public void LT(int v) {
  QueueCMD("LT");
}

public void RT(int v) {
  QueueCMD("RT");
}

public void ST(int v) {
  QueueCMD("ST");
}

public void TO(int v) {
  String c = "TO ";
  c += int(LeftHip.getValue()) + " ";
  c += int(LeftAnkle.getValue()) + " ";
  c += int(RightAnkle.getValue()) + " ";
  c += int(RightHip.getValue()) + " ";
  c += int(ToDur.getValue());
  QueueCMD(c);
}

public void AddToAnim(int v) {
  String c = "TO ";
  c += int(LeftHip.getValue()) + " ";
  c += int(LeftAnkle.getValue()) + " ";
  c += int(RightAnkle.getValue()) + " ";
  c += int(RightHip.getValue()) + " ";
  c += int(ToDur.getValue());
  ListBoxItem lbi = anim.addItem(c, anim.getListBoxItems().length);
}

public void PlayAnim(int v) {
  for (String[] s : anim.getListBoxItems()) {
    QueueCMD(s[0]); 
  }
}

public void ClearAnim(int v) {
  anim.clear(); 
  selectedStep = -1;
}

public void RemoveStep(int v) {
  if (selectedStep > -1) {
    String[] items = new String[anim.getListBoxItems().length - 1];
    int id = 0;
    for (int i=0; i<anim.getListBoxItems().length; i++) {
       if (i != selectedStep) {
         items[id] = (anim.getItem(i).getText());
         id++;
       }
    }
    
    anim.clear();
    
    anim.addItems(items);
    
    selectedStep = -1; 
  }
}

public void PromoteStep(int v) {
  if (selectedStep > 0) {
    String[] items = new String[anim.getListBoxItems().length];
    for (int i=0; i<anim.getListBoxItems().length; i++) {
      items[i] = (anim.getItem(i).getText());
    }
    
    // swap items
    String tmp = items[selectedStep];
    items[selectedStep] = items[--selectedStep];
    items[selectedStep] = tmp;
    
    // update listbox
    anim.clear();
    anim.addItems(items);
    
    
    // update selection
    anim.getItem(selectedStep).setColorBackground(color(255,0,0));
  }
}

public void DemoteStep(int v) {
  if (selectedStep < anim.getListBoxItems().length-1) {
    String[] items = new String[anim.getListBoxItems().length];
    for (int i=0; i<anim.getListBoxItems().length; i++) {
      items[i] = (anim.getItem(i).getText());
    }
    
    // swap items
    String tmp = items[selectedStep];
    items[selectedStep] = items[++selectedStep];
    items[selectedStep] = tmp;
    
    // update listbox
    anim.clear();
    anim.addItems(items);
    
    
    // update selection
    anim.getItem(selectedStep).setColorBackground(color(255,0,0));
  }
}

void controlEvent(ControlEvent theEvent) {
  if(theEvent.isGroup() && theEvent.name().equals("Animation")){
    int step = (int)theEvent.group().value();
    println(step);
 
    // reset background on current selection
    if (selectedStep > -1) {
        println("clear old background");
       anim.getItem(selectedStep).setColorBackground(color(0,0,0));
    }    
    
    selectedStep = step;
    println("set background");
    anim.getItem(selectedStep).setColorBackground(color(255,0,0));
    
    // extract values and apply to joint sliders
    String[] params = split(anim.getItem(selectedStep).getText(), ' ');
    LeftHip.setValue(int(params[1]));
    LeftAnkle.setValue(int(params[2]));
    RightAnkle.setValue(int(params[3]));
    RightHip.setValue(int(params[4]));
    ToDur.setValue(int(params[5]));
  }
}

public void ResetJoints(int v) {
  LeftHip.setValue(0);
  LeftAnkle.setValue(0);
  RightAnkle.setValue(0);
  RightHip.setValue(0);
}
