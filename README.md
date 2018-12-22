This is intended to be a reader/editor for Epub & Mobi documents. Please note that it is in 
a VERY early stage so there is no point in downloading it yet.

This will be implemented using Qt 5.0 which should also allow it to be moved to
Macs, IOS, Android, Linux and Windows with a little work. It is being developed on
Fedora Linux.

The editor will read both EPUB2.0 and EPUB3.0 documents but will only save in the
EPUB3.0 (or 3.1) format. And presumably EPUB1.0 files if I can ever find any to
test against.

As I have many books which are part of a series I have opted to include series data
in the metadata. As I also use Calibre I will use the calibre tags to save this
information as there do not appear to be any standard way of representing this
data.

If anyone has any epub files  that store this or in fact any other data in any
standard-ish form I will attempt to handle this data if you send me examples to
<simonmeaden@virginmedia.com.>

At this point I am only working on the EPUB format, but I intend to handle MOBI at
least, later.

The editor is implemented as a series of plugins rather that tied into the core code.
EPubDocuments & later MobiDocuments are both going to be plugins ('EBookInterface'), the
spellcheckers will be plugins ('SpellingInterface') and the idea is that almost everything
except core file loading will be handled via plugins which would allow a lot of flexibility.
