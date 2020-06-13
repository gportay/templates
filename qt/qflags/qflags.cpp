/*
 * Copyright 2020 Gaël PORTAY
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QFlags>
#include <QString>
#include <QDebug>

enum SfdiskAttribute {
	None               = 0x00,
	RequiredPartition  = 0x01,
	NoBlockIOProtocol  = 0x02,
	LegacyBIOSBootable = 0x04,
};

Q_DECLARE_FLAGS(SfdiskFlags, SfdiskAttribute)
Q_DECLARE_OPERATORS_FOR_FLAGS(SfdiskFlags)

int main(int argc, char * const argv[])
{
	SfdiskFlags flags;
	for (int i = 0; i < argc; i++) {
		ulong f = QString(argv[i]).toULong();
		flags |= SfdiskAttribute(f);
	}
	qDebug() << flags;
}
