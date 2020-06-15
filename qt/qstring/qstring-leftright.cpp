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
		qDebug() << s << "left of @" << s.left(s.indexOf('@'));
		qDebug() << s << "right of @" << s.right(s.indexOf('@'));
	}
}
