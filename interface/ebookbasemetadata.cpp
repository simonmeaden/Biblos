#include "ebookbasemetadata.h"

EBookBaseMetadata::EBookBaseMetadata()
{
  EBookModified modified;
}

EBookCalibre::EBookCalibre()
  : m_modified(false)
{}

QString
EBookCalibre::seriesName() const
{
  return m_series_name;
}

void
EBookCalibre::setSeriesName(const QString& series_name)
{
  m_series_name = series_name;
}

QString
EBookCalibre::seriesIndex() const
{
  return m_series_index;
}

void
EBookCalibre::setSeriesIndex(const QString& series_index)
{
  m_series_index = series_index;
}

QString
EBookCalibre::titleSort() const
{
  return m_title_sort;
}

void
EBookCalibre::setTitleSort(const QString& title_sort)
{
  m_title_sort = title_sort;
}

QString
EBookCalibre::authorLinkMap() const
{
  return m_author_link_map;
}

void
EBookCalibre::setAuthorLinkMap(const QString& author_link_map)
{
  m_author_link_map = author_link_map;
}

QString
EBookCalibre::timestamp() const
{
  return m_timestamp;
}

void
EBookCalibre::setTimestamp(const QString& timestamp)
{
  m_timestamp = timestamp;
}

QString
EBookCalibre::rating() const
{
  return m_rating;
}

void
EBookCalibre::setRating(const QString& rating)
{
  m_rating = rating;
}

QString
EBookCalibre::publicationType() const
{
  return m_publication_type;
}

void
EBookCalibre::setPublicationType(const QString& publication_type)
{
  m_publication_type = publication_type;
}

QString
EBookCalibre::userMetadata() const
{
  return m_user_metadata;
}

void
EBookCalibre::setUserMetadata(const QString& user_metadata)
{
  m_user_metadata = user_metadata;
}

QString
EBookCalibre::userCategories() const
{
  return m_user_categories;
}

void
EBookCalibre::setUserCategories(const QString& user_categories)
{
  m_user_categories = user_categories;
}

QString
EBookCalibre::customMetadata() const
{
  return m_custom_metadata;
}

void
EBookCalibre::setCustomMetadata(const QString& custom_metadata)
{
  m_custom_metadata = custom_metadata;
}

bool
EBookCalibre::isModified() const
{
  return m_modified;
}

void
EBookCalibre::setModified(bool modified)
{
  m_modified = modified;
}
