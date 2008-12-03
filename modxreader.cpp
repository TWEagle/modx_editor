#include "modxreader.h"
#include "modxdata.h"


ModXReader::ModXReader()
{
}

ModXData *ModXReader::data() const
{
	return m_data;
}


void ModXReader::readUnknownElement()
{
	Q_ASSERT(isStartElement());

	while (!atEnd())
	{
		readNext();
		if (isEndElement())
		{
			break;
		}
		if (isStartElement())
		{
			readUnknownElement();
		}
	}
}

bool ModXReader::read(QIODevice *device)
{
	m_data = new ModXData;

	setDevice(device);

	while (!atEnd())
	{
		readNext();
		if (isStartElement())
		{
			if (name() == "mod")
			{
				readModX();
			}
			else
			{
				raiseError(QObject::tr("The file is not a ModX file."));
			}
		}
	}

	return !error();
}

void ModXReader::readModX()
{
	Q_ASSERT(isStartElement() && name() == "mod");
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "header")
			{
				readHeader();
			}
			else if (name() == "action-group")
			{
				readActionGroup();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

void ModXReader::readHeader()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			QString lang = attributes().value("lang").toString();
			if (name() == "title")
			{
				m_data->title.insert(lang, readElementText());
			}
			else if (name() == "description")
			{
				m_data->description.insert(lang, readElementText());
			}
			else if (name() == "author-notes")
			{
				m_data->authorNotes.insert(lang, readElementText());
			}
			else if (name() == "license")
			{
				m_data->license = readElementText();
			}
			else if (name() == "installation")
			{
				readInstallation();
			}
			else if (name() == "author-group")
			{
				readAuthorGroup();
			}
			else if (name() == "history")
			{
				readHistory();
			}
			else if (name() == "mod-version")
			{
				m_data->version = readElementText();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

void ModXReader::readAuthorGroup()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "author")
			{
				m_data->authorGroup << readAuthor();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

Author ModXReader::readAuthor()
{
	Author author;

	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "realname")
			{
				author.realName = readElementText();
			}
			else if (name() == "username")
			{
				author.userName = readElementText();
			}
			else if (name() == "homepage")
			{
				author.homePage = readElementText();
			}
			else if (name() == "contributions")
			{
				author.contributionStatus = attributes().value("status").toString();
				author.contributionFrom = QDate::fromString(attributes().value("from").toString(), Qt::ISODate);
				author.contributionTo = QDate::fromString(attributes().value("to").toString(), Qt::ISODate);
				readElementText();
			}
			else
			{
				readUnknownElement();
			}
		}
	}

	return author;
}

void ModXReader::readInstallation()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "level")
			{
				m_data->installLevel = readElementText();
			}
			else if (name() == "time")
			{
				bool ok = false;
				quint32 time = readElementText().toUInt(&ok);
				m_data->installTime = ok ? time : 60;
			}
			else if (name() == "target-version")
			{
				m_data->targetVersion = readElementText();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

void ModXReader::readHistory()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "entry")
			{
				m_data->history << readEntry();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

ChangelogEntry ModXReader::readEntry()
{
	ChangelogEntry entry;

	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "date")
			{
				entry.date = QDate::fromString(readElementText(), Qt::ISODate);
			}
			else if (name() == "rev-version")
			{
				entry.version = readElementText();
			}
			else if (name() == "changelog")
			{
				QString lang = attributes().value("lang").toString();
				entry.changes.insert(lang, readChanges());
			}
			else
			{
				readUnknownElement();
			}
		}
	}
	return entry;
}

QStringList ModXReader::readChanges()
{
	QStringList changes;
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "change")
			{
				changes << readElementText();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
	return changes;
}


void ModXReader::readActionGroup()
{
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			if (name() == "sql")
			{
				m_data->sql << readElementText();
			}
			else if (name() == "copy")
			{
				m_data->copyFiles = true;
				// Read <file> as unknown elements...
				readUnknownElement();
			}
			else if (name() == "diy-instructions")
			{
				QString lang = attributes().value("lang").toString();
				m_data->diy[lang] = readElementText();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}

/*void ModXReader::readActionGroup()
{
qDebug() << "readActionGroup();";
	while (!atEnd())
	{
		readNext();

		if (isEndElement())
		{
			break;
		}

		if (isStartElement())
		{
			readUnknownElement();
		}
	}
qDebug() << "<-readActionGroup();";
}*/

QDebug operator<<(QDebug dbg, const QXmlStreamAttribute &a)
{
	dbg.nospace() << a.namespaceUri().toString() << ":" << a.name().toString() << " = " << a.value().toString();

	return dbg.space();
}/**/