package com.yahoo;

public class Yahoo_Model {
  private String date;
  private float open;
  private float high;
  private float low;
  private float close;
  private int volume;
  private int openInt;
  
//  Yahoo_Model(String date, float open, float high, float low, float close, int volume, int openInt){
//  Yahoo_Model(){  
//  this.date = date;
//	  this.open = open;
//	  this.high = high;
//	  this.low = low;
//	  this.close = close;
//	  this.volume = volume;
//	  this.openInt = openInt;
//  }
  @Override
  public String toString() { 
      return this.date + this.open + this.high + this.low + this.low + this.close + this.volume + this.openInt;
   } 

  public String getDate() {
    return date;
  }

  public void setDate(String date) {
    this.date = date;
  }

  public float getOpen() {
    return open;
  }

  public void setOpen(float open) {
    this.open = open;
  }

  public float getHigh() {
    return high;
  }

  public void setHigh(float high) {
    this.high = high;
  }

  public float getLow() {
    return low;
  }

  public void setLow(float low) {
    this.low = low;
  }

  public float getClose() {
    return close;
  }

  public void setClose(float close) {
    this.close = close;
  }

  public int getVolume() {
    return volume;
  }

  public void setVolume(int volume) {
    this.volume = volume;
  }

  public int getOpenInt() {
    return openInt;
  }

  public void setOpenInt(int openInt) {
    this.openInt = openInt;
  }
}
