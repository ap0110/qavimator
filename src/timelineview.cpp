/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree   *
 *   Zi Ree @ SecondLife   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qscrollview.h>
#include <qlayout.h>
#include <qpainter.h>

#include "timelineview.h"
#include "timeline.h"
#include "animation.h"

TimelineView::TimelineView(QWidget* parent,const char* name,WFlags f) : QFrame(parent,name,f)
{
  QHBoxLayout* layout=new QHBoxLayout(this);

  layout->setAutoAdd(TRUE);

  timelineTracks=new TimelineTracks(this,"timeline_tracks");

  view=new QScrollView(this,"timeline_inner_view",WNoAutoErase|WStaticContents);

  view->setVScrollBarMode(QScrollView::AlwaysOff);
  view->setHScrollBarMode(QScrollView::AlwaysOff);
  view->enableClipper(TRUE);
  timeline=new Timeline(view->viewport(),"timeline");

//  layout->addWidget(timelineTracks);
//  layout->addWidget(view);

  connect(timeline,SIGNAL(resized(const QSize&)),this,SLOT(doResize(const QSize&)));
  connect(timeline,SIGNAL(animationChanged(Animation*)),this,SLOT(setAnimation(Animation*)));

  marker=new TimelineMarker(view->viewport(),"timeline_marker");

  view->addChild(timeline);
  view->addChild(marker);
}

TimelineView::~TimelineView()
{
}

void TimelineView::scrollTo(int x)
{
  int currX=x-view->contentsX();

  if(currX < (view->width()/4))
//    view->center(x-view->width()/4,view->contentsY());
    view->scrollBy(-KEY_WIDTH,0);
  else if(currX > (view->width()*3/4))
//    view->center(x+view->width()*3/4,view->contentsY());
    view->scrollBy(KEY_WIDTH,0);

  view->moveChild(marker,x+KEY_WIDTH/2,0);
}

void TimelineView::doResize(const QSize& newSize)
{
  view->resizeContents(newSize.width(),newSize.height());
  marker->resize(1,view->height());
}

void TimelineView::setAnimation(Animation* anim)
{
  timelineTracks->setAnimation(anim);
}

Timeline* TimelineView::getTimeline() const
{
  return timeline;
}

// --------------

TimelineTracks::TimelineTracks(QWidget* parent,const char* name,WFlags f) : QWidget(parent,name,f)
{
  resize(LEFT_STRUT,(NUM_PARTS-1)*LINE_HEIGHT);
}

TimelineTracks::~TimelineTracks()
{
}

QSize TimelineTracks::sizeHint() const
{
  return QSize(LEFT_STRUT,(NUM_PARTS-1)*LINE_HEIGHT);
}

void TimelineTracks::paintEvent(QPaintEvent*)
{
  repaint();
}

void TimelineTracks::repaint()
{
  resize(LEFT_STRUT,(NUM_PARTS-1)*LINE_HEIGHT);
  QPainter p(this);

  for(int part=1;part<NUM_PARTS;part++)
  {
    QString trackName=animation->getPartName(part);
    if(trackName!="Site")
    {
//qDebug(trackName);

      int y=(part-1)*LINE_HEIGHT+2;
      p.setPen(QColor("#000000"));
      p.eraseRect(0,y,width(),LINE_HEIGHT);

      // always draw track name
      p.drawText(0,y+KEY_HEIGHT,trackName);
    }
  } // for
}

void TimelineTracks::setAnimation(Animation* anim)
{
  animation=anim;
}

// --------------

TimelineMarker::TimelineMarker(QWidget* parent,const char* name,WFlags f) : QWidget(parent,name,f)
{
  setPaletteBackgroundColor(QColor("#ff0000"));
}

TimelineMarker::~TimelineMarker()
{
}
