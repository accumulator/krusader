/***************************************************************************
                            krdetailedview.h
                           -------------------
  copyright            : (C) 2000-2002 by Shie Erlich & Rafi Yanai
  e-mail               : krusader@users.sourceforge.net
  web site             : http://krusader.sourceforge.net
---------------------------------------------------------------------------
Description
***************************************************************************

A

   db   dD d8888b. db    db .d8888.  .d8b.  d8888b. d88888b d8888b.
   88 ,8P' 88  `8D 88    88 88'  YP d8' `8b 88  `8D 88'     88  `8D
   88,8P   88oobY' 88    88 `8bo.   88ooo88 88   88 88ooooo 88oobY'
   88`8b   88`8b   88    88   `Y8b. 88~~~88 88   88 88~~~~~ 88`8b
   88 `88. 88 `88. 88b  d88 db   8D 88   88 88  .8D 88.     88 `88.
   YP   YD 88   YD ~Y8888P' `8888Y' YP   YP Y8888D' Y88888P 88   YD

                                                   H e a d e r    F i l e

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#ifndef KRDETAILEDVIEW_H
#define KRDETAILEDVIEW_H

#include <klistview.h>
#include <ksqueezedtextlabel.h>
#include <klocale.h>
#include <qwidget.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qdict.h>
#include "krview.h"
#include "krviewitem.h"

class QDragMoveEvent;
class KrRenameTimerObject;
class ListPanel;

/**
 * KrDetailedView implements everthing and anything regarding a detailed view in a filemananger.
 * IT MUST USE KrViewItem as the children to it's *KListView. KrDetailedView and KrViewItem are
 * tightly coupled and the view will not work with other kinds of items.
 * Apart from this, the view is self-reliant and you can use the vast interface to get whatever
 * information is necessery from it.
 */
class KrDetailedView : public KListView, public KrView {
   Q_OBJECT
#define MAX_COLUMNS 9
   friend class KrDetailedViewItem;

public:
   enum ColumnType { Name = 0x0, Extention = 0x1, Mime = 0x2, Size = 0x3, DateTime = 0x4,
                     Permissions = 0x5, KrPermissions = 0x6, Owner = 0x7, Group = 0x8, Unused = 0x9 };

   KrDetailedView( QWidget *parent, ListPanel *panel, bool &left, KConfig *cfg = krConfig, const char *name = 0 );
   virtual ~KrDetailedView();
   virtual int column( ColumnType type );
   virtual KrViewItem *getFirst() { return dynamic_cast<KrViewItem*>( firstChild() ); }
   virtual KrViewItem *getLast() { return dynamic_cast<KrViewItem*>( lastChild() ); }
   virtual KrViewItem *getNext( KrViewItem *current ) { return dynamic_cast<KrViewItem*>( dynamic_cast<KListViewItem*>( current ) ->itemBelow() ); }
   virtual KrViewItem *getPrev( KrViewItem *current ) { return dynamic_cast<KrViewItem*>( dynamic_cast<KListViewItem*>( current ) ->itemAbove() ); }
   virtual KrViewItem *getCurrentKrViewItem() { return dynamic_cast<KrViewItem*>( currentItem() ); }
   virtual KrViewItem *getKrViewItemAt( const QPoint &vp );
   virtual KrViewItem *findItemByName( const QString &name ) { return dynamic_cast<KrViewItem*>( findItem( name, 0 ) ); }
   virtual void addItems( vfs *v, bool addUpDir = true );
	virtual void addItem(vfile *vf);
	virtual void delItem(const QString &name);
	virtual void updateItem(vfile *vf);
 	virtual QString getCurrentItem() const;
   virtual void makeItemVisible( const KrViewItem *item ) { ensureItemVisible( dynamic_cast<const QListViewItem*>( item ) ); }
   virtual void setCurrentItem( const QString& name );
   virtual void updateView() { triggerUpdate(); emit selectionChanged(); }
   virtual void clear();
   virtual void sort() { KListView::sort(); }
   virtual void setSortMode( SortSpec mode );
   virtual void prepareForActive();
   virtual void prepareForPassive();
   virtual void saveSettings() { KListView::saveLayout( _config, nameInKConfig() ); }
   virtual void restoreSettings() { KListView::restoreLayout( _config, nameInKConfig() ); }
   virtual QString nameInKConfig() { return _nameInKConfig; }

signals:
   void executed( QString &name );
   void itemDescription( QString &desc );
   void needFocus();
   void contextMenu( const QPoint &point );
   void letsDrag( QStringList items, QPixmap icon );
   void gotDrop( QDropEvent *e );
   void renameItem( const QString &oldName, const QString &newName );
   void middleButtonClicked( QListViewItem *item );

protected:
   void newColumn( ColumnType type );
   virtual void keyPressEvent( QKeyEvent *e );
   virtual void contentsMousePressEvent( QMouseEvent *e );
   virtual void contentsMouseMoveEvent ( QMouseEvent * e );
   virtual void contentsWheelEvent( QWheelEvent *e );
   virtual bool acceptDrag( QDropEvent* e ) const;
   virtual void contentsDropEvent( QDropEvent *e );
   virtual void contentsDragMoveEvent( QDragMoveEvent *e );
   virtual void startDrag();
   virtual bool event( QEvent *e );
	virtual void initProperties();
   //virtual void focusOutEvent( QFocusEvent * );

protected slots:
   void rename( QListViewItem *item, int c );
   void slotClicked( QListViewItem *item );
   void slotDoubleClicked( QListViewItem *item );
   void slotItemDescription( QListViewItem *item );
   void slotCurrentChanged( QListViewItem *item );
   void handleContextMenu( QListViewItem*, const QPoint&, int );
   virtual void renameCurrentItem();
   void inplaceRenameFinished( QListViewItem *it, int col );
   void quickSearch( const QString &, int = 0 );
   void stopQuickSearch( QKeyEvent* );
   void handleQuickSearchEvent( QKeyEvent* );
   void setNameToMakeCurrent( QListViewItem *it );

   /**
    * used internally to produce the signal middleButtonClicked()
    */
   void slotMouseClicked( int button, QListViewItem * item, const QPoint & pos, int c );
   inline void slotExecuted( QListViewItem* i ) {
      QString tmp = dynamic_cast<KrViewItem*>( i ) ->name();
      emit executed( tmp );
   }

public slots:
   void refreshColors();

private:
   ColumnType _columns[ MAX_COLUMNS ];
   static QString ColumnName[ MAX_COLUMNS ];
   bool _focused;
   bool caseSensitiveSort;
   KrViewItem *_currDragItem;
   QString _nameInKConfig;
   bool &_left;

   bool singleClicked;
   bool modifierPressed;
   QTime clickTime;
   QListViewItem *clickedItem;
   QTimer renameTimer;
	QDict<KrDetailedViewItem> dict;
};

#endif /* KRDETAILEDVIEW_H */
