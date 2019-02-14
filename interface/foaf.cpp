#include "foaf.h"

const QString Foaf::m_prefix = "foaf: http://xmlns.com/foaf/spec/";

Foaf::Foaf()
{
  m_term = NO_TERM;
}

Foaf::Term Foaf::term()
{
  return m_term;
}

QString Foaf::asString()
{
  return toString(m_term);
}

bool Foaf::isFoaf()
{
  if (m_term != NO_TERM)
    return true;
  return false;
}

QString Foaf::toString(Foaf::Term term)
{
  switch (term) {
  case AGENT:
    return "foaf:agent";
  case PERSON:
    return "foaf:person";
  case NAME:
    return "foaf:name";
  case TITLE:
    return "foaf:title";
  case IMG:
    return "foaf:img";
  case DEPICTION:
    return "foaf:depiction";
  case DEPICTS:
    return "foaf:depicts";
  case FAMILY_NAME:
    return "foaf:familyname";
  case GIVEN_NAME:
    return "foaf:givenname";
  case KNOWS:
    return "foaf:knows";
  case BASED_NEAR:
    return "foaf:based_near";
  case AGE:
    return "foaf:age";
  case MADE:
    return "foaf:made";
  case MAKER:
    return "foaf:maker";
  case PRIMARY_TOPIC:
    return "foaf:primaryTopic";
  case PRIMARY_TOPIC_OF:
    return "foaf:primaryTopicOf";
  case PROJECT:
    return "foaf:Project";
  case ORGANISATION:
    return "foaf:Organization";
  case GROUP:
    return "foaf:Group";
  case MEMBER:
    return "foaf:member";
  case DOCUMENT:
    return "foaf:Document";
  case IMAGE:
    return "foaf:Image";
  case NICK:
    return "foaf:nick";
  case MBOX:
    return "foaf:mbox";
  case HOMEPAGE:
    return "foaf:homepage";
  case WEBLOG:
    return "foaf:weblog";
  case OPENID:
    return "foaf:openid";
  case JABBER_ID:
    return "foaf:jabberID";
  case MBOX_SHA1SUM:
    return "foaf:mbox_sha1sum";
  case INTEREST:
    return "foaf:interest";
  case TOPIC_INTEREST:
    return "foaf:topic_interest";
  case TOPIC:
    return "foaf:topic";
  case PAGE:
    return "foaf:page";
  case WORKPLACE_HOMEPAGE:
    return "foaf:workplaceHomepage";
  case WORK_INFO_HOMEPAGE:
    return "foaf:workInfoHomepage";
  case SCHOOL_HOMEPAGE:
    return "foaf:schoolHomepage";
  case PUBLICATIONS:
    return "foaf:publications";
  case CURRENT_PROJECT:
    return "foaf:currentProject";
  case PAST_PROJECT:
    return "foaf:pastProject";
  case ACCOUNT:
    return "foaf:account";
  case ONLINE_ACCOUNT:
    return "foaf:OnlineAccount";
  case ACCOUNT_NAME:
    return "foaf:accountName";
  case ACCOUNT_SERVICE_HOMEPAGE:
    return "foaf:accountServiceHomepage";
  case PERSONAL_PROFILE_DOCUMENT:
    return "foaf:PersonalProfileDocument";
  case TIPJAR:
    return "foaf:tipjar";
  case SHA1:
    return "foaf:sha1";
  case THUMBNAIL:
    return "foaf:thumbnail";
  case LOGO:
    return "foaf:logo";

  default:
    return "no_term";
  }
}

Foaf Foaf::fromString(QString term_name)
{
  Foaf terms;
  QString name;
  if (term_name.toLower().startsWith("foaf:")) {
    QStringList splits = term_name.toLower().split(":");
    if (splits.size() == 2) {
      name = splits.at(1).toLower();
    } else {
      terms.setTerm(Term::NO_TERM);
    }
  }

  if (name == "agent") {
    terms.setTerm(AGENT);
  } else if (name == "person") {
    terms.setTerm(PERSON);
  } else if (name == "name") {
    terms.setTerm(NAME);
  } else if (name == "title") {
    terms.setTerm(TITLE);
  } else if (name == "img") {
    terms.setTerm(IMG);
  } else if (name == "depiction") {
    terms.setTerm(DEPICTION);
  } else if (name == "depicts") {
    terms.setTerm(DEPICTS);
  } else if (name == "familyname") {
    terms.setTerm(FAMILY_NAME);
  } else if (name == "givenname") {
    terms.setTerm(GIVEN_NAME);
  } else if (name == "knows") {
    terms.setTerm(KNOWS);
  } else if (name == "based_near") {
    terms.setTerm(BASED_NEAR);
  } else if (name == "age") {
    terms.setTerm(AGE);
  } else if (name == "made") {
    terms.setTerm(MADE);
  } else if (name == "maker") {
    terms.setTerm(MAKER);
  } else if (name == "primarytopic") {
    terms.setTerm(PRIMARY_TOPIC);
  } else if (name == "primarytopicof") {
    terms.setTerm(PRIMARY_TOPIC_OF);
  } else if (name == "project") {
    terms.setTerm(PROJECT);
  } else if (name == "organization") {
    terms.setTerm(ORGANISATION);
  } else if (name == "group") {
    terms.setTerm(GROUP);
  } else if (name == "member") {
    terms.setTerm(MEMBER);
  } else if (name == "document") {
    terms.setTerm(DOCUMENT);
  } else if (name == "image") {
    terms.setTerm(IMAGE);
  } else if (name == "nick") {
    terms.setTerm(NICK);
  } else if (name == "mbox") {
    terms.setTerm(MBOX);
  } else if (name == "homepage") {
    terms.setTerm(HOMEPAGE);
  } else if (name == "weblog") {
    terms.setTerm(WEBLOG);
  } else if (name == "openid") {
    terms.setTerm(OPENID);
  } else if (name == "jabberid") {
    terms.setTerm(JABBER_ID);
  } else if (name == "mbox_sha1sum") {
    terms.setTerm(MBOX_SHA1SUM);
  } else if (name == "interest") {
    terms.setTerm(INTEREST);
  } else if (name == "topic_interest") {
    terms.setTerm(TOPIC_INTEREST);
  } else if (name == "topic") {
    terms.setTerm(TOPIC);
  } else if (name == "page") {
    terms.setTerm(PAGE);
  } else if (name == "workplacehomepage") {
    terms.setTerm(WORKPLACE_HOMEPAGE);
  } else if (name == "workinfohomepage") {
    terms.setTerm(WORK_INFO_HOMEPAGE);
  } else if (name == "schoolhomepage") {
    terms.setTerm(SCHOOL_HOMEPAGE);
  } else if (name == "publications") {
    terms.setTerm(PUBLICATIONS);
  } else if (name == "currentproject") {
    terms.setTerm(CURRENT_PROJECT);
  } else if (name == "pastproject") {
    terms.setTerm(PAST_PROJECT);
  } else if (name == "account") {
    terms.setTerm(ACCOUNT);
  } else if (name == "onlineaccount") {
    terms.setTerm(ONLINE_ACCOUNT);
  } else if (name == "accountname") {
    terms.setTerm(ACCOUNT_NAME);
  } else if (name == "accountservicehomepage") {
    terms.setTerm(ACCOUNT_SERVICE_HOMEPAGE);
  } else if (name == "personalprofiledocument") {
    terms.setTerm(PERSONAL_PROFILE_DOCUMENT);
  } else if (name == "tipjar") {
    terms.setTerm(TIPJAR);
  } else if (name == "sha1") {
    terms.setTerm(SHA1);
  } else if (name == "thumbnail") {
    terms.setTerm(THUMBNAIL);
  } else if (name == "logo") {
    terms.setTerm(LOGO);
  }
  terms.setCode(name);
  return terms;
}

bool Foaf::isFoaf(QString tag_name)
{
  if (tag_name.toLower().startsWith("foaf:")) {
    Foaf foaf = fromString(tag_name);
    if (foaf.term() != NO_TERM)
      return true;
  }

  return false;
}

QString Foaf::prefix()
{
  return m_prefix;
}

QString Foaf::lang() const
{
  return m_lang;
}

void Foaf::setLang(const QString &lang)
{
  m_lang = lang;
}

QString Foaf::id() const
{
  return m_id;
}

void Foaf::setId(const QString &id)
{
  m_id = id;
}

QString Foaf::content() const
{
  return m_content;
}

void Foaf::setContent(const QString &content)
{
  m_content = content;
}

QString Foaf::about() const
{
  return m_about;
}

void Foaf::setAbout(const QString &about)
{
  m_about = about;
}

QString Foaf::scheme()
{
  return m_scheme;
}

void Foaf::setScheme(QString scheme)
{
  m_scheme = scheme;
}


QString Foaf::code() const
{
  return m_code;
}

void Foaf::setTerm(const Foaf::Term &term)
{
  m_term = term;
}

void Foaf::setCode(const QString &code)
{
  m_code = code;
}
