#ifndef DCTERMS_H
#define DCTERMS_H

#include <QString>
#include <QStringList>

class DCTerms
{
public:
  enum Term {
    NO_TERM = 0,
    ABSTRACT,
    ACCESS_RIGHTS,
    ACCRUAL_METHOD,
    ACCRUAL_PERIODICITY,
    ACCRUAL_POLICY,
    ALTERNATIVE,
    AUDIENCE,
    AVAILABLE,
    BIBLIOGRAPHIC_CITATION,
    CONFORMS_TO,
    CONTRIBUTOR,
    COVERAGE,
    CREATED,
    CREATOR,
    DATE,
    DATE_ACCEPTED,
    DATE_COPYRIGHTED,
    DATE_SUBMITTED,
    DESCRIPTION,
    EDUCATION_LEVEL,
    EXTENT,
    FORMAT,
    HAS_FORMAT,
    HAS_PART,
    HAS_VERSION,
    IDENTIFIER,
    INSTRUCTIONAL_METHOD,
    IS_FORMAT_OF,
    IS_PART_OF,
    IS_REFERENCED_BY,
    IS_REPLACED_BY,
    IS_REQUIRED_BY,
    ISSUED,
    IS_VERSION_OF,
    LANGUAGE,
    LICENSE,
    MEDIATOR,
    MEDIUM,
    MODIFIED,
    PROVENANCE,
    PUBLISHER,
    REFERENCES,
    RELATION,
    REPLACES,
    REQUIRES,
    RIGHTS,
    RIGHTS_HOLDER,
    SOURCE,
    SPACIAL,
    SUBJECT,
    TABLE_OF_CONTENTS,
    TEMPORAL,
    TITLE,
    TYPE,
    VALID,
  };

  DCTerms();

  Term term();

  QString asString();
  bool isDCTerm();

  static QString toString(Term term);
  static DCTerms fromString(QString term_name);
  static bool isDcTerm(QString tag_name);

  QString code() const;

protected:
  Term m_term;
  QString m_code;

  void setTerm(const Term& term);
  void setCode(const QString& code);
};

#endif // DCTERMS_H

// namespace
// 	abstract , accessRights , accrualMethod , accrualPeriodicity , accrualPolicy , alternative ,
// audience , available , bibliographicCitation , conformsTo , contributor , coverage , created ,
// creator , date , dateAccepted , dateCopyrighted , dateSubmitted , description , educationLevel
// , extent , format , hasFormat , hasPart , hasVersion , identifier , instructionalMethod ,
// isFormatOf , isPartOf , isReferencedBy , isReplacedBy , isRequiredBy , issued , isVersionOf ,
// language , license , mediator , medium , modified , provenance , publisher , references ,
// relation , replaces , requires , rights , rightsHolder , source , spatial , subject ,
// tableOfContents , temporal , title , type , valid

// namespace 1.1
// contributor , coverage , creator , date , description , format , identifier , language ,
// publisher , relation , rights , source , subject , title , type
