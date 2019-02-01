#ifndef FOAF_H
#define FOAF_H

#include <QString>
#include <QStringList>

class Foaf
{
public:
  enum Term {
    NO_TERM,
    AGENT,
    PERSON,
    NAME,
    TITLE,
    IMG,
    DEPICTION, //()
    DEPICTS,
    FAMILY_NAME,
    GIVEN_NAME,
    KNOWS,
    BASED_NEAR,
    AGE,
    MADE, //(maker)
    MAKER,
    PRIMARY_TOPIC, //(primaryTopicOf)
    PRIMARY_TOPIC_OF,
    PROJECT,
    ORGANISATION,
    GROUP,
    MEMBER,
    DOCUMENT,
    IMAGE,
    NICK,
    MBOX,
    HOMEPAGE,
    WEBLOG,
    OPENID,
    JABBER_ID,
    MBOX_SHA1SUM,
    INTEREST,
    TOPIC_INTEREST,
    TOPIC, // (page)
    PAGE,
    WORKPLACE_HOMEPAGE,
    WORK_INFO_HOMEPAGE,
    SCHOOL_HOMEPAGE,
    PUBLICATIONS,
    CURRENT_PROJECT,
    PAST_PROJECT,
    ACCOUNT,
    ONLINE_ACCOUNT,
    ACCOUNT_NAME,
    ACCOUNT_SERVICE_HOMEPAGE,
    PERSONAL_PROFILE_DOCUMENT,
    TIPJAR,
    SHA1,
    THUMBNAIL,
    LOGO,
  };

  Foaf();

  Term term();

  QString asString();
  bool isFoaf();

  QString code() const;

  static QString toString(Term term);
  static Foaf fromString(QString term_name);
  static bool isFoaf(QString tag_name);
  static QString prefix();
  static QString scheme();

protected:
  Term m_term;
  QString m_code;
  static const QString m_prefix;
  static const QString m_scheme;

  void setTerm(const Term& term);
  void setCode(const QString& code);
};

#endif // FOAF_H

// Core

// Agent
// Person
// name
// title
// img
// depiction (depicts)
// familyName
// givenName
// knows
// based_near
// age
// made (maker)
// primaryTopic (primaryTopicOf)
// Project
// Organization
// Group
// member
// Document
// Image

// Social Web

// nick
// mbox
// homepage
// weblog
// openid
// jabberID
// mbox_sha1sum
// interest
// topic_interest
// topic (page)
// workplaceHomepage
// workInfoHomepage
// schoolHomepage
// publications
// currentProject
// pastProject
// account
// OnlineAccount
// accountName
// accountServiceHomepage
// PersonalProfileDocument
// tipjar
// sha1
// thumbnail
// logo
