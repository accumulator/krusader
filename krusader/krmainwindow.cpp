/*****************************************************************************
 * Copyright (C) 2010 Jan Lepper <dehtris@yahoo.de>                          *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This package is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this package; if not, write to the Free Software               *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA *
 *****************************************************************************/

#include "krmainwindow.h"

#include <QtGlobal>
#include <QAction>
#include <kactioncollection.h>

#include <stdio.h>


// void KrMainWindow::enableAction(const char *name, bool enable)
// {
//     QAction *act = actions()->action(name);
//     if(act)
//         act->setEnabled(enable);
//     else
//         fprintf(stderr, "no such action: \"%s\"\n", name);
// }
// 
// QAction* KrMainWindow::action(const char *name)
// {
//     QAction *act = actions()->action(name);
//     if(!act)
//         qFatal("no such action: \"%s\"", name);
//     return act;
// }
