/***************************************************************************
                          krviewer.h  -  description
                             -------------------
    begin                : Thu Apr 18 2002
    copyright            : (C) 2002 by Shie Erlich & Rafi Yanai
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KRVIEWER_H
#define KRVIEWER_H

#include <qwidget.h>
#include <kparts/mainwindow.h>
#include <ktempfile.h>
#include <kparts/partmanager.h>
#include <kparts/browserextension.h>

/**
  *@author Shie Erlich & Rafi Yanai
  */

class QPopupMenu;

class KrViewer : public KParts::MainWindow  {
   Q_OBJECT
public: 
  ~KrViewer();
  static void view(KURL url);
  static void edit(KURL url, bool create = false );


public slots:
  bool viewGeneric();
  void viewHex();
  bool viewText();

  bool editGeneric(QString mimetype, KURL _url);
  bool editText( bool create = false );

  void keyPressEvent(QKeyEvent *e);
  void createGUI(KParts::Part*);

  void handleOpenURLRequest( const KURL &url, const KParts::URLArgs & );

protected:
  virtual bool queryClose();
  virtual bool queryExit();

private:
  KrViewer(QWidget *parent=0, const char *name=0);
  KParts::Part* getPart(KURL url, QString m ,bool readOnly, bool create=false);
  QPopupMenu* viewerMenu;

  KURL url;
  KParts::PartManager manager;

  KParts::ReadOnlyPart  *generic_part;
  KParts::ReadOnlyPart  *text_part;
  KParts::ReadOnlyPart  *hex_part;
  KParts::ReadWritePart *editor_part;

  KTempFile tmpFile;
};

#endif
