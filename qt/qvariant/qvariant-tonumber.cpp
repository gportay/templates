/*
 * Copyright 2020 Gaël PORTAY
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QVariant>
#include <QDebug>

int main(int argc, char * const argv[])
{
	for (int i = 1; i < argc; i++) {
		QVariant v(argv[i]);
		qDebug() << v << "to int               " << v.toInt();
		qDebug() << v << "to unsigned int      " << v.toUInt();
		qDebug() << v << "to long long         " << v.toLongLong();
		qDebug() << v << "to unsigned long long" << v.toULongLong();
	}
}
