import processing.serial.*;
import controlP5.*;
import java.util.Arrays;
import java.io.BufferedWriter;
import java.io.FileWriter;

ControlP5 cp5;
PFont f;

Button connectBut;
Slider progSlider;

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
    .setPosition(0, 190)
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
    println("sending: "+c + "\r\n");
    port.write(c + "\r\n");
    lastCmd = c;
    //cp5.get(Textfield.class, "LastCommand").setText(c);
  }
}

public void QueueCMD(String c) {
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
  
  if (s.length() > 1 && s.substring(0,2).equals("OK")) cts = true;
  serialLog.append(s);
  //appendTextToFile("serial.log",s);
  while (serialLog.size () > 25) serialLog.remove(0);
}


void keyPressed() {
  if (key=='1') {
  }
}


void draw() {
  background(255);
  textFont(f, 15);

try {
  // Draw serialLog
  fill(0);
  for (int i=0; i<serialLog.size (); i++) {
    String s = serialLog.get(i);
    text(s, 330, 20 + i*16);
  }

  // send cmds
  if (sendBuffer.size() > 0 && cts) {
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
     //l.align(LEFT, CENTER);
     //l.setPadding(10,8);
     
     c.setColorBackground(color(0,0,0, 255));
     c.setColorLabel(color(255));
     c.setColorActive(color(0,0,0, 255));
     c.setColorForeground(color(0,0,0, 255));
     
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

