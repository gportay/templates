/*
 * Copyright 2020 Gaël PORTAY
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QString>
#include <QDebug>

int main(int argc, char * const argv[])
{
	for (int i = 1; i < argc; i++) {
		QString s(argv[i]);
		qDebug() << s << "to int               " << s.toInt(nullptr, 0);
		qDebug() << s << "to unsigned int      " << s.toUInt(nullptr, 0);
		qDebug() << s << "to long              " << s.toLong(nullptr, 0);
		qDebug() << s << "to unsigned long     " << s.toULong(nullptr, 0);
		qDebug() << s << "to long long         " << s.toLongLong(nullptr, 0);
		qDebug() << s << "to unsigned long long" << s.toULongLong(nullptr, 0);
	}
}
