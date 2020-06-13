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

const QMap<QString, SfdiskAttribute > SfdiskAttributes = {
	{ "RequiredPartition",  RequiredPartition  },
	{ "NoBlockIOProtocol",  NoBlockIOProtocol  },
	{ "LegacyBIOSBootable", LegacyBIOSBootable },
};

static SfdiskAttribute toSfdiskAttribute(const QString& s)
{
	bool ok;
	ulong l = s.toLong(&ok);
	if (ok)
		return SfdiskAttribute(l);

	return SfdiskAttributes[s];
}

int main(int argc, char * const argv[])
{
	SfdiskFlags flags;
	for (int i = 0; i < argc; i++)
		flags |= toSfdiskAttribute(argv[i]);
	qDebug() << flags;
}
