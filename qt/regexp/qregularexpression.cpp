/*
 * Copyright 2020 Gaël PORTAY
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QRegularExpressionMatch>
#include <QDebug>

int main(int argc, char * const argv[])
{
	QRegularExpression re(argv[1]);
	for (int i = 2; i < argc; i++) {
		QRegularExpressionMatch match = re.match(argv[i]);
		qDebug() << argv[1] << argv[i] << "hasMatch:" << match.hasMatch();
	}
}
