#include "dcterms.h"

DCTerms::DCTerms()
{
  m_term = NO_TERM;
}

DCTerms::Term DCTerms::term()
{
  return m_term;
}

void DCTerms::setTerm(const DCTerms::Term& term)
{
  m_term = term;
}

/*!
 * \brief Returns the Term as a string.
 *
 * Functionally the same as calling the static method DCTerms::toString(this_term)
 * where this_term is an existing DCTerms object.
 *
 * \return the code string.
 */
QString DCTerms::asString()
{
  return toString(m_term);
}

bool DCTerms::isDCTerm()
{
  if (m_term != NO_TERM)
    return true;
  return false;
}

QString DCTerms::toString(DCTerms::Term term)
{
  switch (term) {
  case ABSTRACT:
    return "dcterms:abstract";
  case ACCESS_RIGHTS:
    return "dcterms:accessRights";
  case ACCRUAL_METHOD:
    return "dcterms:accrualMethod";
  case ACCRUAL_PERIODICITY:
    return "dcterms:accrualPeriodicity";
  case ACCRUAL_POLICY:
    return "dcterms:accrualPolicy";
  case ALTERNATIVE:
    return "dcterms:alternative";
  case AUDIENCE:
    return "dcterms:audience";
  case AVAILABLE:
    return "dcterms:available";
  case BIBLIOGRAPHIC_CITATION:
    return "dcterms:bibliographicCitation";
  case CONFORMS_TO:
    return "dcterms:conformsTo";
  case CONTRIBUTOR:
    return "dcterms:contributor";
  case COVERAGE:
    return "dcterms:coverage";
  case CREATED:
    return "dcterms:created";
  case CREATOR:
    return "dcterms:creator";
  case DATE:
    return "dcterms:date";
  case DATE_ACCEPTED:
    return "dcterms:dateAccepted";
  case DATE_COPYRIGHTED:
    return "dcterms:dateCopyrighted";
  case DATE_SUBMITTED:
    return "dcterms:dateSubmitted";
  case DESCRIPTION:
    return "dcterms:description";
  case EDUCATION_LEVEL:
    return "dcterms:educationLevel";
  case EXTENT:
    return "dcterms:extent";
  case FORMAT:
    return "dcterms:format";
  case HAS_FORMAT:
    return "dcterms:hasFormat";
  case HAS_PART:
    return "dcterms:hasPart";
  case HAS_VERSION:
    return "dcterms:hasVersion";
  case IDENTIFIER:
    return "dcterms:identifier";
  case INSTRUCTIONAL_METHOD:
    return "dcterms:instructionalMethod";
  case IS_FORMAT_OF:
    return "dcterms:isFormatOf";
  case IS_PART_OF:
    return "dcterms:isPartOf";
  case IS_REFERENCED_BY:
    return "dcterms:isReferencedBy";
  case IS_REPLACED_BY:
    return "dcterms:isReplacedBy";
  case IS_REQUIRED_BY:
    return "dcterms:isRequiredBy";
  case ISSUED:
    return "dcterms:issued";
  case IS_VERSION_OF:
    return "dcterms:isVersionOf";
  case LANGUAGE:
    return "dcterms:language";
  case LICENSE:
    return "dcterms:license";
  case MEDIATOR:
    return "dcterms:mediator";
  case MEDIUM:
    return "dcterms:medium";
  case MODIFIED:
    return "dcterms:modified";
  case PROVENANCE:
    return "dcterms:provenance";
  case PUBLISHER:
    return "dcterms:publisher";
  case REFERENCES:
    return "dcterms:references";
  case RELATION:
    return "dcterms:relation";
  case REPLACES:
    return "dcterms:replaces";
  case REQUIRES:
    return "dcterms:requires";
  case RIGHTS:
    return "dcterms:rights";
  case RIGHTS_HOLDER:
    return "dcterms:rightsHolder";
  case SOURCE:
    return "dcterms:source";
  case SPACIAL:
    return "dcterms:spatial";
  case SUBJECT:
    return "dcterms:subject";
  case TABLE_OF_CONTENTS:
    return "dcterms:tableOfContents";
  case TEMPORAL:
    return "dcterms:temporal";
  case TITLE:
    return "dcterms:title";
  case TYPE:
    return "dcterms:type";
  case VALID:
    return "dcterms:valid";

  default:
    return "no_term";
  }
}

QString DCTerms::code() const
{
  return m_code;
}

void DCTerms::setCode(const QString& code)
{
  m_code = code;
}

DCTerms DCTerms::fromString(QString term_name)
{
  DCTerms terms;
  QString name;
  if (term_name.startsWith("dcterms:")) {
    QStringList splits = term_name.split(":");
    if (splits.size() == 2) {
      name = splits.at(1);
    } else {
      terms.setTerm(Term::NO_TERM);
    }
  }
  if (name == "abstract") {
    terms.setTerm(ABSTRACT);
  } else if (name == "accessRights") {
    terms.setTerm(ACCESS_RIGHTS);
  } else if (name == "accrualMethod") {
    terms.setTerm(ACCRUAL_METHOD);
  } else if (name == "accrualPeriodicity") {
    terms.setTerm(ACCRUAL_PERIODICITY);
  } else if (name == "accrualPolicy") {
    terms.setTerm(ACCRUAL_POLICY);
  } else if (name == "alternative") {
    terms.setTerm(ALTERNATIVE);
  } else if (name == "audience") {
    terms.setTerm(AUDIENCE);
  } else if (name == "available") {
    terms.setTerm(AVAILABLE);
  } else if (name == "bibliographicCitation") {
    terms.setTerm(BIBLIOGRAPHIC_CITATION);
  } else if (name == "conformsTo") {
    terms.setTerm(CONFORMS_TO);
  } else if (name == "contributor") {
    terms.setTerm(CONTRIBUTOR);
  } else if (name == "coverage") {
    terms.setTerm(COVERAGE);
  } else if (name == "created") {
    terms.setTerm(CREATED);
  } else if (name == "creator") {
    terms.setTerm(CREATOR);
  } else if (name == "date") {
    terms.setTerm(DATE);
  } else if (name == "dateAccepted") {
    terms.setTerm(DATE_ACCEPTED);
  } else if (name == "dateCopyrighted") {
    terms.setTerm(DATE_COPYRIGHTED);
  } else if (name == "dateSubmitted") {
    terms.setTerm(DATE_SUBMITTED);
  } else if (name == "description") {
    terms.setTerm(DESCRIPTION);
  } else if (name == "educationLevel") {
    terms.setTerm(EDUCATION_LEVEL);
  } else if (name == "extent") {
    terms.setTerm(EXTENT);
  } else if (name == "format") {
    terms.setTerm(FORMAT);
  } else if (name == "hasFormat") {
    terms.setTerm(HAS_FORMAT);
  } else if (name == "hasPart") {
    terms.setTerm(HAS_PART);
  } else if (name == "hasVersion") {
    terms.setTerm(HAS_VERSION);
  } else if (name == "identifier") {
    terms.setTerm(IDENTIFIER);
  } else if (name == "instructionalMethod") {
    terms.setTerm(INSTRUCTIONAL_METHOD);
  } else if (name == "isFormatOf") {
    terms.setTerm(IS_FORMAT_OF);
  } else if (name == "isPartOf") {
    terms.setTerm(IS_PART_OF);
  } else if (name == "isReferencedBy") {
    terms.setTerm(IS_REFERENCED_BY);
  } else if (name == "isReplacedBy") {
    terms.setTerm(IS_REPLACED_BY);
  } else if (name == "isRequiredBy") {
    terms.setTerm(IS_REQUIRED_BY);
  } else if (name == "issued") {
    terms.setTerm(ISSUED);
  } else if (name == "isVersionOf") {
    terms.setTerm(IS_VERSION_OF);
  } else if (name == "language") {
    terms.setTerm(LANGUAGE);
  } else if (name == "license") {
    terms.setTerm(LICENSE);
  } else if (name == "mediator") {
    terms.setTerm(MEDIATOR);
  } else if (name == "medium") {
    terms.setTerm(MEDIUM);
  } else if (name == "modified") {
    terms.setTerm(MODIFIED);
  } else if (name == "provenance") {
    terms.setTerm(PROVENANCE);
  } else if (name == "publisher") {
    terms.setTerm(PUBLISHER);
  } else if (name == "references") {
    terms.setTerm(REFERENCES);
  } else if (name == "relation") {
    terms.setTerm(RELATION);
  } else if (name == "replaces") {
    terms.setTerm(REPLACES);
  } else if (name == "requires") {
    terms.setTerm(REQUIRES);
  } else if (name == "rights") {
    terms.setTerm(RIGHTS);
  } else if (name == "rightsHolder") {
    terms.setTerm(RIGHTS_HOLDER);
  } else if (name == "source") {
    terms.setTerm(SOURCE);
  } else if (name == "spatial") {
    terms.setTerm(SPACIAL);
  } else if (name == "subject") {
    terms.setTerm(SUBJECT);
  } else if (name == "tableOfContents") {
    terms.setTerm(TABLE_OF_CONTENTS);
  } else if (name == "temporal") {
    terms.setTerm(TEMPORAL);
  } else if (name == "title") {
    terms.setTerm(TITLE);
  } else if (name == "type") {
    terms.setTerm(TYPE);
  } else if (name == "valid") {
    terms.setTerm(VALID);
  }
  if (terms.term() != NO_TERM) {
    terms.setCode(term_name);
  } else {
    terms.setCode("");
  }
  return terms;
}

bool DCTerms::isDcTerm(QString tag_name)
{
  if (tag_name.toLower().startsWith("dcterms:")) {
    DCTerms terms = fromString(tag_name);
    if (terms.term() != NO_TERM)
      return true;
  }

  return false;
}
