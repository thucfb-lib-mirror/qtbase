/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qfbbackingstore_p.h"
#include "qfbwindow_p.h"
#include "qfbscreen_p.h"

#include <qpa/qplatformwindow.h>
#include <QtGui/qscreen.h>

QT_BEGIN_NAMESPACE

QFbBackingStore::QFbBackingStore(QWindow *window)
    : QPlatformBackingStore(window)
{
    if (window->handle())
        (static_cast<QFbWindow *>(window->handle()))->setBackingStore(this);
    else
        (static_cast<QFbScreen *>(window->screen()->handle()))->addBackingStore(this);
}

QFbBackingStore::~QFbBackingStore()
{
}

void QFbBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
    Q_UNUSED(window);
    Q_UNUSED(offset);

    (static_cast<QFbWindow *>(window->handle()))->repaint(region);
}

void QFbBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
    Q_UNUSED(staticContents);

    if (mImage.size() != size)
        mImage = QImage(size, window()->screen()->handle()->format());
}

const QImage QFbBackingStore::image()
{
    return mImage;
}

void QFbBackingStore::lock()
{
    mImageMutex.lock();
}

void QFbBackingStore::unlock()
{
    mImageMutex.unlock();
}

void QFbBackingStore::beginPaint(const QRegion &)
{
    lock();
}

void QFbBackingStore::endPaint()
{
    unlock();
}

QT_END_NAMESPACE

