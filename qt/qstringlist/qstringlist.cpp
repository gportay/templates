/*
 * Copyright 2020 Gaël PORTAY
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QStringList>
#include <QDebug>

int main(int argc, char * const argv[])
{
	for (int i = 1; i < argc; i++) {
		QStringList l(QString(argv[i]).split(' '));
		qDebug() << l;
	}
}
