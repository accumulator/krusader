/***************************************************************************
                         KgProtocols.cpp  -  description
                             -------------------
    copyright            : (C) 2004 by Csaba Karai
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

                                                     S o u r c e    F i l e

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kgprotocols.h"
#include "../krusader.h"
#include "../krservices.h"
#include <klocale.h>
#include <kprotocolinfo.h>
#include <kmimetype.h>
#include <q3header.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QVBoxLayout>
#include <kiconloader.h>
#include <q3whatsthis.h>

QString KgProtocols::defaultProtocols  = "krarc,iso,tar";
QString KgProtocols::defaultIsoMimes   = "application/x-iso,application/x-cd-image,"
                                         "application/x-dvd-image";
QString KgProtocols::defaultKrarcMimes = "application/x-7z,application/x-7z-compressed,"
                                         "application/x-ace,application/x-ace-compressed,"
                                         "application/x-arj,application/x-arj-compressed,"
                                         "application/x-bzip2,"
                                         "application/x-cpio,application/x-deb,"
                                         "application/x-debian-package,"
                                         "application/x-gzip,application/x-jar,"
                                         "application/x-lha,application/x-lha-compressed,"
                                         "application/x-rar,application/x-rar-compressed,"
                                         "application/x-rpm,"
                                         "application/x-zip,application/x-zip-compressed";
QString KgProtocols::defaultTarMimes   = "application/x-tar,application/x-tarz,"
                                         "application/x-compressed-tar,"
                                         "application/x-tbz,application/x-tgz";

KgProtocols::KgProtocols( bool first, QWidget* parent ) :
      KonfiguratorPage( first, parent )
{
  QGridLayout *KgProtocolsLayout = new QGridLayout( this );
  KgProtocolsLayout->setSpacing( 6 );

  //  -------------------------- LINK VIEW ----------------------------------
  
  QGroupBox *linkGrp = createFrame( i18n( "Links" ), this );    
  QGridLayout *linkGrid = createGridLayout( linkGrp );
  
  linkList = new Q3ListView( linkGrp, "linkList" );
  linkList->addColumn( i18n( "Defined Links" ) );
  linkList->header()->setStretchEnabled( true, 0 );
  linkList->setRootIsDecorated( true );
  
  linkGrid->addWidget( linkList, 0, 0 );
  KgProtocolsLayout->addWidget( linkGrp, 0, 0, 2, 1 );

  //  -------------------------- BUTTONS ----------------------------------

  QWidget *vbox1Widget = new QWidget( this );
  QVBoxLayout *vbox1 = new QVBoxLayout( vbox1Widget );
  
  addSpacer( vbox1 );
  btnAddProtocol = new QPushButton( vbox1Widget );
  btnAddProtocol->setPixmap( krLoader->loadIcon( "1leftarrow", KIconLoader::Small ) );
  Q3WhatsThis::add( btnAddProtocol, i18n( "Add protocol to the link list." ) );
  vbox1->addWidget( btnAddProtocol );

  btnRemoveProtocol = new QPushButton( vbox1Widget );
  btnRemoveProtocol->setPixmap( krLoader->loadIcon( "1rightarrow", KIconLoader::Small ) );
  Q3WhatsThis::add( btnRemoveProtocol, i18n( "Remove protocol from the link list." ) );
  vbox1->addWidget( btnRemoveProtocol );
  addSpacer( vbox1 );
  
  KgProtocolsLayout->addWidget( vbox1Widget, 0 ,1 );

  QWidget *vbox2Widget = new QWidget( this );
  QVBoxLayout *vbox2 = new QVBoxLayout( vbox2Widget );
  
  addSpacer( vbox2 );
  btnAddMime = new QPushButton( vbox2Widget );
  btnAddMime->setPixmap( krLoader->loadIcon( "1leftarrow", KIconLoader::Small ) );
  Q3WhatsThis::add( btnAddMime, i18n( "Add mime to the selected protocol on the link list." ) );
  vbox2->addWidget( btnAddMime );

  btnRemoveMime = new QPushButton( vbox2Widget );
  btnRemoveMime->setPixmap( krLoader->loadIcon( "1rightarrow", KIconLoader::Small ) );
  Q3WhatsThis::add( btnRemoveMime, i18n( "Remove mime from the link list." ) );
  vbox2->addWidget( btnRemoveMime );
  addSpacer( vbox2 );
  
  KgProtocolsLayout->addWidget( vbox2Widget, 1 ,1 );
  
  //  -------------------------- PROTOCOLS LISTBOX ----------------------------------

  QGroupBox *protocolGrp = createFrame( i18n( "Protocols" ), this );    
  QGridLayout *protocolGrid = createGridLayout( protocolGrp );
  
  protocolList = new Q3ListBox( protocolGrp, "protocolList" );
  loadListCapableProtocols();
  protocolGrid->addWidget( protocolList, 0, 0 );

  KgProtocolsLayout->addWidget( protocolGrp, 0 ,2 );

  //  -------------------------- MIMES LISTBOX ----------------------------------

  QGroupBox *mimeGrp = createFrame( i18n( "Mimes" ), this );    
  QGridLayout *mimeGrid = createGridLayout( mimeGrp );
  
  mimeList = new Q3ListBox( mimeGrp, "protocolList" );
  loadMimes();
  mimeGrid->addWidget( mimeList, 0, 0 );

  KgProtocolsLayout->addWidget( mimeGrp, 1 ,2 );
  
  //  -------------------------- CONNECT TABLE ----------------------------------  
  
  connect( protocolList,      SIGNAL( selectionChanged() ), this, SLOT( slotDisableButtons() ) );
  connect( linkList,          SIGNAL( selectionChanged() ), this, SLOT( slotDisableButtons() ) );
  connect( mimeList,          SIGNAL( selectionChanged() ), this, SLOT( slotDisableButtons() ) );
  connect( linkList,          SIGNAL( currentChanged( Q3ListViewItem *) ),   this, SLOT( slotDisableButtons() ) );
  connect( btnAddProtocol,    SIGNAL( clicked() )         , this, SLOT( slotAddProtocol() ) );
  connect( btnRemoveProtocol, SIGNAL( clicked() )         , this, SLOT( slotRemoveProtocol() ) );
  connect( btnAddMime,        SIGNAL( clicked() )         , this, SLOT( slotAddMime() ) );
  connect( btnRemoveMime,     SIGNAL( clicked() )         , this, SLOT( slotRemoveMime() ) );
  
  loadInitialValues();
  slotDisableButtons();
}
  
void KgProtocols::addSpacer( QBoxLayout *layout )
{
  layout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
}

void KgProtocols::loadListCapableProtocols()
{
  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  
  for ( QStringList::Iterator it = protocols.begin(); it != protocols.end();) 
  {
//    if( !KProtocolInfo::supportsListing( *it ) )    // TODO TODO
//    {
//      it = protocols.remove( it );
//      continue;
//    }
    ++it;
  }
  protocolList->insertStringList( protocols );
}

void KgProtocols::loadMimes()
{
  KMimeType::List mimes = KMimeType::allMimeTypes();
  
  for( KMimeType::List::const_iterator it = mimes.begin(); it != mimes.end(); it++ )
    mimeList->insertItem( (*it)->name() );
    
  mimeList->sort();
}

void KgProtocols::slotDisableButtons()
{
  btnAddProtocol->setEnabled( protocolList->selectedItem() != 0 );
  Q3ListViewItem *listViewItem = linkList->currentItem();
  bool isProtocolSelected = ( listViewItem == 0 ? false : listViewItem->parent() == 0 );
  btnRemoveProtocol->setEnabled( isProtocolSelected );
  btnAddMime->setEnabled( listViewItem != 0 && mimeList->selectedItem() != 0 );
  btnRemoveMime->setEnabled( listViewItem == 0 ? false : listViewItem->parent() != 0 );
  
  if( linkList->currentItem() == 0 && linkList->firstChild() != 0 )
    linkList->setCurrentItem( linkList->firstChild() );
  if( linkList->selectedItem() == 0 && linkList->currentItem() != 0 )
    linkList->setSelected( linkList->currentItem(), true );
}

void KgProtocols::slotAddProtocol()
{
  Q3ListBoxItem *item = protocolList->selectedItem();
  if( item )
  {
    addProtocol( item->text(), true );
    slotDisableButtons();
    emit sigChanged();
  }
}

void KgProtocols::addProtocol( QString name, bool changeCurrent )
{
  Q3ListBoxItem *item = protocolList->findItem( name, Q3ListBox::ExactMatch );
  if( item )
  {
    protocolList->removeItem( protocolList->index( item ) );
    Q3ListViewItem *listViewItem = new Q3ListViewItem( linkList, name );
    listViewItem->setPixmap( 0, krLoader->loadIcon( "exec", KIconLoader::Small ) );
    
    if( changeCurrent )
      linkList->setCurrentItem( listViewItem );
  }
}

void KgProtocols::slotRemoveProtocol()
{
  Q3ListViewItem *item = linkList->currentItem();
  if( item )
  {
    removeProtocol( item->text( 0 ) );
    slotDisableButtons();
    emit sigChanged();
  }
}

void KgProtocols::removeProtocol( QString name )
{
  Q3ListViewItem *item = linkList->findItem( name, 0 );
  if( item )
  {
    while( item->childCount() != 0 )
      removeMime( item->firstChild()->text( 0 ) );
     
    linkList->takeItem( item );
    protocolList->insertItem( name );
    protocolList->sort();
  }
}

void KgProtocols::slotAddMime()
{
  Q3ListBoxItem *item = mimeList->selectedItem();
  if( item && linkList->currentItem() != 0 )
  {
    Q3ListViewItem *itemToAdd = linkList->currentItem();
    if( itemToAdd->parent() )
      itemToAdd = itemToAdd->parent();
      
    addMime( item->text(), itemToAdd->text( 0 ) );
    slotDisableButtons();
    emit sigChanged();
  }
}

void KgProtocols::addMime( QString name, QString protocol )
{
  Q3ListBoxItem *item = mimeList->findItem( name, Q3ListBox::ExactMatch );
  Q3ListViewItem *currentListItem = linkList->findItem( protocol, 0 );
  
  if( item && currentListItem && currentListItem->parent() == 0 )
  {
    mimeList->removeItem( mimeList->index( item ) );
    Q3ListViewItem *listViewItem = new Q3ListViewItem( currentListItem, name );
    listViewItem->setPixmap( 0, krLoader->loadIcon( "mime", KIconLoader::Small ) );
    currentListItem->setOpen( true );
  }
}

void KgProtocols::slotRemoveMime()
{
  Q3ListViewItem *item = linkList->currentItem();
  if( item )
  {
    removeMime( item->text( 0 ) );
    slotDisableButtons();
    emit sigChanged();
  }
}

void KgProtocols::removeMime( QString name )
{
  Q3ListViewItem *currentMimeItem = linkList->findItem( name, 0 );
  
  if( currentMimeItem && currentMimeItem->parent() != 0 )
  {
    mimeList->insertItem( currentMimeItem->text( 0 ) );
    mimeList->sort();
    currentMimeItem->parent()->takeItem( currentMimeItem );
  }
}

void KgProtocols::loadInitialValues()
{
  while( linkList->childCount() != 0 )
    removeProtocol( linkList->firstChild()->text( 0 ) );
  
  KConfigGroup group( krConfig, "Protocols" );
  QStringList protList = group.readEntry( "Handled Protocols", QStringList() );
    
  for( QStringList::Iterator it = protList.begin(); it != protList.end(); it++ ) 
  {
    addProtocol( *it );
    
    QStringList mimes = group.readEntry( QString( "Mimes For %1" ).arg( *it ), QStringList() );
    
    for( QStringList::Iterator it2 = mimes.begin(); it2 != mimes.end(); it2++ )
      addMime( *it2, *it );
  }
  
  if( linkList->firstChild() != 0 )
    linkList->setCurrentItem( linkList->firstChild() );
  slotDisableButtons();
}

void KgProtocols::setDefaults()
{
  while( linkList->childCount() != 0 )
    removeProtocol( linkList->firstChild()->text( 0 ) );
  
  addProtocol( "iso" );
  QStringList isoMimes = QStringList::split( ',', defaultIsoMimes );
  for( QStringList::Iterator it = isoMimes.begin(); it != isoMimes.end(); it++ )
    addMime( *it, "iso" );
  
  addProtocol( "krarc" );
  QStringList krarcMimes = QStringList::split( ',', defaultKrarcMimes );
  for( QStringList::Iterator it = krarcMimes.begin(); it != krarcMimes.end(); it++ )
    addMime( *it, "krarc" );
  
  addProtocol( "tar" );
  QStringList tarMimes = QStringList::split( ',', defaultTarMimes );
  for( QStringList::Iterator it = tarMimes.begin(); it != tarMimes.end(); it++ )
    addMime( *it, "tar" );
    
  slotDisableButtons();
    
  if( isChanged() )
    emit sigChanged();
}

bool KgProtocols::isChanged()
{
  KConfigGroup group( krConfig, "Protocols" );
  QStringList protList = group.readEntry( "Handled Protocols", QStringList() );
  
  if( (int)protList.count() != linkList->childCount() )
    return true;
  
  Q3ListViewItem *item = linkList->firstChild();
  while( item )
  {
    if( !protList.contains( item->text( 0 ) ) )
      return true;
      
    QStringList mimes = group.readEntry( QString( "Mimes For %1" ).arg( item->text( 0 ) ), QStringList() );
    
    if( (int)mimes.count() != item->childCount() )
      return true;
    Q3ListViewItem *childs = item->firstChild();
    while( childs )
    {
      if( !mimes.contains( childs->text( 0 ) ) )
        return true;
      childs = childs->nextSibling();
    }
      
    item = item->nextSibling();
  }
    
  return false;
}

bool KgProtocols::apply()
{
  KConfigGroup group( krConfig, "Protocols" );
  
  QStringList protocolList;
  
  Q3ListViewItem *item = linkList->firstChild();
  while( item )
  {
    protocolList.append( item->text( 0 ) );
    
    QStringList mimes;
    Q3ListViewItem *childs = item->firstChild();
    while( childs )
    {
      mimes.append( childs->text( 0 ) );
      childs = childs->nextSibling();
    }
    group.writeEntry( QString( "Mimes For %1" ).arg( item->text( 0 ) ), mimes );
    
    item = item->nextSibling();
  }  
  group.writeEntry( "Handled Protocols", protocolList );
  krConfig->sync();  
  
  KrServices::clearProtocolCache();
  
  emit sigChanged();  
  return false;
}

void KgProtocols::init()
{
  if( !krConfig->groupList().contains( "Protocols" ) )
  {
    KConfigGroup group( krConfig, "Protocols" );
    group.writeEntry( "Handled Protocols", defaultProtocols );
    group.writeEntry( "Mimes For iso",     defaultIsoMimes );
    group.writeEntry( "Mimes For krarc",   defaultKrarcMimes );
    group.writeEntry( "Mimes For tar",     defaultTarMimes );
  }
}

#include "kgprotocols.moc"
