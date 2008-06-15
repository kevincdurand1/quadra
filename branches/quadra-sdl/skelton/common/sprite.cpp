/* -*- Mode: C++; c-basic-offset: 2; tab-width: 2; indent-tabs-mode: nil -*-
 * 
 * Quadra, an action puzzle game
 * Copyright (C) 1998-2000  Ludus Design
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sprite.h"

#include "types.h"
#include "bitmap.h"
#include "video.h"
#include "SDL_endian.h"

using std::max;

#define FONT_SIZE (141-32)

Sprite::Sprite(const Bitmap& b):
	Bitmap(b[0], b.width, b.height, b.realwidth, true) {
}

Sprite* Sprite::New(const Bitmap& b) {
  return new Sprite(b);
}

void Sprite::draw(const Bitmap& d, int dx, int dy) const {
	if(d.clip(dx, dy, this))
		return;
	for(int y=clip_y1; y<=clip_y2; y++) {
		for(int i=clip_x1; i<=clip_x2; i++) {
			Byte pel = *(operator[](y-dy)+(i-dx));
			// optimization since the mask is always 0
			// because of Svgalib
			if(pel)
			  d.fast_pel(i, y, pel);
		}
	}
}

Fontdata::Fontdata(Res &res, int s) {
	shrink = s;
	int w, h, rw;
	for(int i=0; i<FONT_SIZE; i++) {
		res.read(&w, sizeof(int));
                w = SDL_SwapLE32(w);
		if(w != 0) {
			res.read(&h, sizeof(int));
                        h = SDL_SwapLE32(h);
			res.read(&rw, sizeof(int));
                        rw = SDL_SwapLE32(rw);
			Bitmap tmp(w, h, rw);
			res.read(tmp[0], rw*h);
			spr[i] = Sprite::New(tmp);
			pre_width[i] = max(spr[i]->width - shrink, 3);
		} else {
			spr[i] = NULL;
			pre_width[i] = 0;
		}
	}
}

Fontdata::Fontdata(const Fontdata &o) {
	for(int i=0; i<FONT_SIZE; i++) {
		if(o.spr[i]) 
			spr[i] = Sprite::New(*o.spr[i]);
		else
			spr[i] = NULL;
		pre_width[i] = o.pre_width[i];
	}
	shrink = o.shrink;
}

Fontdata::~Fontdata() {
	for(int i=0; i<FONT_SIZE; i++)
		if(spr[i])
			delete spr[i];
}

int Fontdata::width(const char *m) const {
	int c;
	int wid = 0;
	while(*m) {
		c = translate(&m);
		if(c < 0)
			c = 'i'-33;
		wid += pre_width[c];
	}
	return wid+shrink;
}

int Fontdata::width(const char *m, int num) const {
	int c;
	int wid = 0;
	while(*m && num--) {
		c = translate(&m);
		if(c < 0)
			c = 'i'-33;
		wid += pre_width[c];
	}
	return wid+shrink;
}


int Fontdata::translate(const char **m) const {
	int ret;
	char c;
	c = *(*m)++;
	if(c == 32)
		return -1;  // special code for space
	if(c > 32 && c < 127)
		return c-33; // standard value
	switch(c) {
		// ascii 183 followed with a number to do a glyph
		case '�':
			ret=(int) (*(*m)++)-48 + 133;
			if(ret<133 || ret>137)
				return -2;
			break;
		case '�': ret='C'; break;
		case '�': ret=139; break;
		case '�': ret=140; break;
		case '�': ret='u'; break;
		case '�': ret=127; break;
		case '�': ret=128; break;
		case '�': ret=129; break;
		case '�': ret=130; break;
		case '�': ret=131; break;
		case '�':
		case '�': ret='a'; break;
		case '�': ret=132; break;
		case '�':
		case '�':
		case '�': ret='U'; break;
		case '�':
		case '�':
		case '�': ret='A'; break;
		case '�':
		case '�':
		case '�':
		case '�': ret='E'; break;
		case '�': ret=138; break;
		case '�':
		case '�': ret='o'; break;
		case '�': ret='y'; break;
		case '�':
		case '�':
		case '�': ret='O'; break;
		case '�': ret=141; break;
		case '�':
		case '�': ret='i'; break;
		case '�':
		case '�':
		case '�': ret='I'; break;
		default: return -2;
	}
	return ret-33;
}

Font::Font(const Fontdata& f):
	fdata_original(f),
	fdata(f) {
}

Font::Font(const Fontdata& f, const Palette& dst, int r, int g, int b, int r2, int g2, int b2):
	fdata_original(f),
	fdata(f) {
	colorize(dst, r,g,b,r2,g2,b2);
}

void Font::colorize(const Palette& dst, int r, int g, int b, int r2, int g2, int b2) {
	Byte i;
	Remap *map = new Remap(dst);
	for(i=0; i<8; i++)
		map->findrgb(i, (Byte) ((r2*(7-i)+r*i)/7), (Byte) ((g2*(7-i)+g*i)/7), (Byte) ((b2*(7-i)+b*i)/7));
	remap(map);
	delete map;
}

void Font::remap(const Remap *map) {
	for(int i=0; i<FONT_SIZE; i++) {
		Sprite *spr = fdata_original.spr[i];
		if(spr) {
			for(int y=0; y<spr->height; y++)
				for(int x=0; x<spr->width; x++) {
					Byte pel = *((*spr)[y]+x);
					if(pel != 0)
						fdata.spr[i]->fast_pel(x, y, map->map[pel]);
				}
		}
	}
}

void Font::draw(const char *m, const Bitmap& b, int x, int y) const {
	int c;
	if(x == CENTER) {
		x = (b.width - fdata.width(m)) >> 1;
	}
	while(*m) {
		c = fdata.translate(&m);
		if(c < 0) {
			c = 'i'-33;
		} else {
			if(fdata.spr[c])
				fdata.spr[c]->draw(b, x, y);
		}
		x += fdata.pre_width[c];
	}
}

void Font::draw(const char *m, const Video_bitmap& b, int x, int y) const {
	int c;
	if(x == CENTER) {
		x = (b.width - fdata.width(m)) >> 1;
	}
	while(*m) {
		c = fdata.translate(&m);
		if(c < 0) {
			c = 'i'-33;
		} else {
			if(fdata.spr[c])
				b.put_sprite(*(fdata.spr[c]), x, y);
		}
		x += fdata.pre_width[c];
	}
}
