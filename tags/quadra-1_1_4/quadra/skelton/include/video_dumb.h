/* -*- Mode: C++; c-basic-offset: 2; tab-width: 2; indent-tabs-mode: nil -*-
 * Copyright (c) 1998-2000 Ludus Design enr.
 * All Rights Reserved.
 * Tous droits r�serv�s.
 */

#ifndef _HEADER_VIDEO_DUMB
#define _HEADER_VIDEO_DUMB

#include "video.h"

class Dumb_Video_bitmap: public Video_bitmap {
public:
  int opos_y;
  static Dumb_Video_bitmap* New(const int px, const int py,
				   const int w, const int h,
				   const int rw);
  static Dumb_Video_bitmap* New(const int px, const int py,
				   const int w, const int h);
  Dumb_Video_bitmap(const int px, const int py, const int w, const int h,
		       const int rw);
  Dumb_Video_bitmap(const int px, const int py, const int w, const int h);
  virtual void rect(const int x, const int y,
		    const int w, const int h, const int color) const;
  virtual void box(const int x, const int y, const int w, const int h,
		   const int color) const;
  virtual void get_bitmap(const Bitmap *bit, const int x, const int y,
			  const int w, const int h) const;
  virtual void put_pel(const int x, const int y, const Byte c) const;
  virtual void hline(const int y, const int x,
		     const int w, const Byte c) const;
  virtual void vline(const int x, const int y,
		     const int w, const Byte c) const;
  virtual void line(const int x1, const int y1,
		    const int x2, const int y2, const Byte c) const;
  virtual void put_bitmap(const Bitmap& d, const int dx, const int dy) const;
  virtual void put_sprite(const Sprite& d, const int dx, const int dy) const;
  virtual void setmem();
};

class Video_Dumb: public Video {
public:
  int displayoffset;
  static Video_Dumb* New(int w, int h, int b, const char *wname);
  Video_Dumb(int w, int h, int b, const char *wname);
  virtual ~Video_Dumb();
  virtual void lock();
  virtual void unlock();
  virtual void flip();
  virtual void start_frame();
  virtual void end_frame();
  virtual void setpal(const Palette& p);
  virtual void dosetpal(PALETTEENTRY pal[256], int size);
  virtual void restore();
  virtual void clean_up();
  virtual void snap_shot(int x, int y, int w, int h);
};

#endif /* _HEADER_VIDEO_DUMB */
