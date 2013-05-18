#!/usr/bin/env python  
import subprocess as sub  

configProc = "work/configWorkers"
task = "task0"
def configSet( cap ):
  cmd=[]
  cmd.append( configProc );
  cmd.append( cap );
  cmd.append( "-t" );
  cmd.append( task );
  return cmd

def configSetQueue( qName ):
  cmd = configSet( "SetQueue" )
  cmd.append("-c")
  cmd.append(qName)
  sub.Popen(cmd)
  
def configStart( num ):
  cmd = configSet( "Start" )
  cmd.append("-n")
  cmd.append(str(num))
  sub.Popen(cmd)

def configDef():
  cmd = configSet( "SetDefaultKeeper" )
  sub.Popen(cmd)

def configHello():
  cmd = configSet( "SetCap" )
  cmd.append("-c")
  cmd.append("libHello.so")
  sub.Popen(cmd)

def configHelloV():
  cmd = configSet( "SetCap" )
  cmd.append("-c")
  cmd.append("libHelloV.so")
  sub.Popen(cmd)

configDef()
configHello()
configHelloV()
configSetQueue("queue0")
configStart(4)
