/***************************************************************************
 *   Copyright (C) 2009 by the phpBB Group                                 *
 *   phpbb.com                                                             *
 *                                                                         *
 *  This file is part of MODX Editor.                                      *
 *                                                                         *
 *  MODX Editor is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  MODX Editor is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with MODX Editor.  If not, see <http://www.gnu.org/licenses/>.   *
 ***************************************************************************/

#ifndef CHANGELOGPAGE_H
#define CHANGELOGPAGE_H

#include "page.h"
#include "modxdata.h"
#include "listmodel.h"

#include "ui_changelogpage.h"

class ChangelogSortFilterProxyModel;

class ChangelogPage : public Page {
    Q_OBJECT
    Q_DISABLE_COPY(ChangelogPage)
public:
	explicit ChangelogPage(QWidget *parent = 0);

	virtual void setData(const ModXData *data);
	virtual void getData(ModXData *data);

protected slots:
	void updateEntry(const QItemSelection &, const QItemSelection &);
	void updateVersion(const QString &newName);

	void addVersion();
	void removeVersion();

protected:
    virtual void changeEvent(QEvent *e);

private:
	ListModel<ChangelogEntry> *model;
	ChangelogSortFilterProxyModel *proxyModel;

	Ui::ChangelogPage ui;
};

#endif // CHANGELOGPAGE_H
