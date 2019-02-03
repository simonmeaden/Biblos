#include "marcrelator.h"

MarcRelator::MarcRelator()
{
  m_type = NO_TYPE;
}

MarcRelator::Relator MarcRelator::type()
{
  return m_type;
}

void MarcRelator::setType(MarcRelator::Relator type)
{
  m_type = type;
}

QString MarcRelator::code()
{
  return m_code;
}

void MarcRelator::setCode(QString code)
{
  m_code = code;
}

QString MarcRelator::asString()
{
  return MarcRelator::toString(m_type);
}

bool MarcRelator::isRelator()
{
  if (m_type != NO_TYPE)
    return true;
  return false;
}

QString MarcRelator::toString(MarcRelator::Relator relator)
{
  switch (relator) {
  case MarcRelator::ABRIDGER:
    return "abr";
  case MarcRelator::ART_COPYIST:
    return "acp";
  case MarcRelator::ACTOR:
    return "act";
  case MarcRelator::ART_DIRECTOR:
    return "adi";
  case MarcRelator::ADAPTER:
    return "adp";
  case MarcRelator::AUTHOR_OF_AFTERWORD:
    return "aft";
  case MarcRelator::ANALYST:
    return "anl";
  case MarcRelator::ANIMATOR:
    return "anm";
  case MarcRelator::ANNOTATOR:
    return "ann";
  case MarcRelator::BIBLIOGRAPHIC_ANTECEDENT:
    return "ant";
  case MarcRelator::APPELLEE:
    return "ape";
  case MarcRelator::APPELLANT:
    return "apl";
  case MarcRelator::APPLICANT:
    return "app";
  case MarcRelator::AUTHOR_IN_QUOTATIONS:
    return "aqt";
  case MarcRelator::ARCHITECT:
    return "arc";
  case MarcRelator::ARTISTIC_DIRECTOR:
    return "ard";
  case MarcRelator::ARRANGER:
    return "arr";
  case MarcRelator::ARTIST:
    return "art";
  case MarcRelator::ASSIGNEE:
    return "asg";
  case MarcRelator::ASSOCIATED_NAME:
    return "asn";
  case MarcRelator::AUTOGRAPHER:
    return "ato";
  case MarcRelator::ATTRIBUTED_NAME:
    return "att";
  case MarcRelator::AUCTIONEER:
    return "auc";
  case MarcRelator::AUTHOR:
    return "aut";
  case MarcRelator::AUTHOR_OF_DIALOG:
    return "aud";
  case MarcRelator::AUTHOR_OF_INTRODUCTION:
    return "aui";
  case MarcRelator::SCREENWRITER:
    return "aus";
  case MarcRelator::BINDING_DESIGNER:
    return "bdd";
  case MarcRelator::BOOKJACKET_DESIGNER:
    return "bjd";
  case MarcRelator::BOOK_DESIGNER:
    return "bkd";
  case MarcRelator::BOOK_PRODUCER:
    return "bkp";
  case MarcRelator::BLURB_WRITER:
    return "blw";
  case MarcRelator::BINDER:
    return "bnd";
  case MarcRelator::BOOKPLATE_DESIGNER:
    return "bpd";
  case MarcRelator::BROADCASTER:
    return "brd";
  case MarcRelator::BRAILE_EMBOSSER:
    return "brl";
  case MarcRelator::BOOKSELLER:
    return "bsl";
  case MarcRelator::CASTER:
    return "cas";
  case MarcRelator::CONCEPTOR:
    return "ccp";
  case MarcRelator::CHOREOGRAPHER:
    return "chr";
  case MarcRelator::COLLABORATOR:
    return "clb"; // Discontinued
  case MarcRelator::CLIENT:
    return "cli";
  case MarcRelator::CALLIGRAPHER:
    return "cll";
  case MarcRelator::COLORIST:
    return "clr";
  case MarcRelator::COLLOTYPER:
    return "clt";
  case MarcRelator::COMMENTATOR:
    return "cmm";
  case MarcRelator::COMPOSER:
    return "cmp";
  case MarcRelator::COMPOSITOR:
    return "cmt";
  case MarcRelator::CONDUCTOR:
    return "cnd";
  case MarcRelator::CINEMATOGRAPHER:
    return "cng";
  case MarcRelator::CENSOR:
    return "cns";
  case MarcRelator::CONTESTANT_APPELLEE:
    return "coe";
  case MarcRelator::CONTESTOR:
    return "col";
  case MarcRelator::COMPILER:
    return "com";
  case MarcRelator::CONSERVATOR:
    return "con";
  case MarcRelator::COLLECTION_REGISTRAR:
    return "cor";
  case MarcRelator::CONTESTANT:
    return "cos";
  case MarcRelator::CONTESTEE_APPELLANT:
    return "cot";
  case MarcRelator::COURT_GOVERNED:
    return "cou";
  case MarcRelator::COVER_DESIGNER:
    return "cov";
  case MarcRelator::COPYRIGHT_CLAIMANT:
    return "cpc";
  case MarcRelator::COMPLAINANT_APPELLEE:
    return "cpe";
  case MarcRelator::COPYRIGHT_HOLDER:
    return "cph";
  case MarcRelator::COMPLAINANT:
    return "cpl";
  case MarcRelator::COMPLAINANT_APPELLANT:
    return "cpt";
  case MarcRelator::CREATOR:
    return "cre";
  case MarcRelator::CORRESPONDANT:
    return "crp";
  case MarcRelator::CORRECTOR:
    return "crr";
  case MarcRelator::COURT_REPORTER:
    return "crt";
  case MarcRelator::CONSULTANT:
    return "csl";
  case MarcRelator::CONSULTANT_TO_A_PROJECT:
    return "csp";
  case MarcRelator::COSTUME_DESIGNER:
    return "cst";
  case MarcRelator::CONTRIBUTOR:
    return "ctb";
  case MarcRelator::CONTESTEE_APPELLEE:
    return "cte";
  case MarcRelator::CARTOGRAPHER:
    return "ctg";
  case MarcRelator::CONTRACTOR:
    return "ctr";
  case MarcRelator::CONTESTEE:
    return "cts";
  case MarcRelator::CONTESTANT_APPELLANT:
    return "ctt";
  case MarcRelator::CURATOR:
    return "cur";
  case MarcRelator::COMMENTATOR_FOR_WRITTEN_TEXT:
    return "cwt";
  case MarcRelator::DISTIBUTION_PLACE:
    return "dbp";
  case MarcRelator::DEFENDANT:
    return "dfd";
  case MarcRelator::DEFENDANT_APPELLEE:
    return "dfe";
  case MarcRelator::DEFENDANT_APPELLANT:
    return "dft";
  case MarcRelator::DEGREE_GRANTING_INSTITUTION:
    return "dgg";
  case MarcRelator::DEGREE_SUPERVISOR:
    return "dgs";
  case MarcRelator::DISSERTANT:
    return "dis";
  case MarcRelator::DELINEATOR:
    return "dln";
  case MarcRelator::DANCER:
    return "dnc";
  case MarcRelator::DONOR:
    return "dnr";
  case MarcRelator::DEPICTED:
    return "dpc";
  case MarcRelator::DEPOSITOR:
    return "dpt";
  case MarcRelator::DRAFTSMAN:
    return "drm";
  case MarcRelator::DIRECTOR:
    return "drt";
  case MarcRelator::DESIGNER:
    return "dsr";
  case MarcRelator::DISTRIBUTOR:
    return "dst";
  case MarcRelator::DATA_CONTRIBUTOR:
    return "dtc";
  case MarcRelator::DEDICATEE:
    return "dte";
  case MarcRelator::DATA_MANAGER:
    return "dtm";
  case MarcRelator::DEDICATOR:
    return "dto";
  case MarcRelator::DUBIOUS_AUTHOR:
    return "dub";
  case MarcRelator::EDITOR_OF_COMPILATION:
    return "edc";
  case MarcRelator::EDITOR_OF_MOVING_IMAGE_WORK:
    return "edm";
  case MarcRelator::EDITOR:
    return "edt";
  case MarcRelator::ENGRAVER:
    return "egr";
  case MarcRelator::ELECTRICIAN:
    return "elg";
  case MarcRelator::ELECTROTYPER:
    return "elt";
  case MarcRelator::ENGINEER:
    return "eng";
  case MarcRelator::ENACTING_JURISTICTION:
    return "enj";
  case MarcRelator::ETCHER:
    return "etr";
  case MarcRelator::EVENT_PLACE:
    return "evp";
  case MarcRelator::EXPERT:
    return "exp";
  case MarcRelator::FACSIMILIST:
    return "fac";
  case MarcRelator::FILM_DISTRIBUTOR:
    return "fds";
  case MarcRelator::FIELD_DIRECTOR:
    return "fld";
  case MarcRelator::FILM_EDITOR:
    return "flm";
  case MarcRelator::FILM_DIRECTOR:
    return "fmd";
  case MarcRelator::FILM_MAKER:
    return "fmk";
  case MarcRelator::FORMER_OWNOR:
    return "fmo";
  case MarcRelator::FILM_PRODUCER:
    return "fmp";
  case MarcRelator::FUNDER:
    return "fnd";
  case MarcRelator::FIRST_PARTY:
    return "fpy";
  case MarcRelator::FORGER:
    return "frg";
  case MarcRelator::GEOGRAPHIC_INFORMATION_SPECIALIST:
    return "gis";
  case MarcRelator::GRAPHIC_TECHNICIAN:
    return "grt"; // Discontinued
  case MarcRelator::HOST_INSTITUTION:
    return "his";
  case MarcRelator::HONOREE:
    return "hnr";
  case MarcRelator::HOST:
    return "hst";
  case MarcRelator::ILLISTRATOR:
    return "ill";
  case MarcRelator::ILLUMINATOR:
    return "ilu";
  case MarcRelator::INSCRIBER:
    return "ins";
  case MarcRelator::INVENTOR:
    return "inv";
  case MarcRelator::ISSUING_BODY:
    return "isb";
  case MarcRelator::INSTRUMENTALIST:
    return "itr";
  case MarcRelator::INTERVIEWEE:
    return "ive";
  case MarcRelator::INTERVIEWER:
    return "ivr";
  case MarcRelator::JUDGE:
    return "jud";
  case MarcRelator::JURISTICTION_GOVERNED:
    return "jug";
  case MarcRelator::LABORATORY:
    return "lbr";
  case MarcRelator::LIBRETTIST:
    return "lbt";
  case MarcRelator::LABORATORY_DIRECTOR:
    return "ldr";
  case MarcRelator::LEAD:
    return "led";
  case MarcRelator::LIBELLEE_APPELEE:
    return "lee";
  case MarcRelator::LIBELLEE:
    return "lel";
  case MarcRelator::LENDER:
    return "len";
  case MarcRelator::LIBELLEE_APPELLANT:
    return "let";
  case MarcRelator::LIGHTING_DESIGNER:
    return "lgd";
  case MarcRelator::LIBELANT_APPELLEE:
    return "lie";
  case MarcRelator::LIBELLANT:
    return "lil";
  case MarcRelator::LIBELANT_APPELLANT:
    return "lit";
  case MarcRelator::LANDSCAPE_ARCHITECT:
    return "lsa";
  case MarcRelator::LICENSEE:
    return "lse";
  case MarcRelator::LICENSOR:
    return "lso";
  case MarcRelator::LITHOGRAPHER:
    return "ltg";
  case MarcRelator::LYRICIST:
    return "lyr";
  case MarcRelator::MUSIC_COPYIST:
    return "mcp";
  case MarcRelator::METADATA_CONTACT:
    return "mdc";
  case MarcRelator::MEDIUM:
    return "med";
  case MarcRelator::MANUFACTURE_PLACE:
    return "mfp";
  case MarcRelator::MANFACTURER:
    return "mfr";
  case MarcRelator::MODERATOR:
    return "mod";
  case MarcRelator::MONITOR:
    return "mon";
  case MarcRelator::MARBLER:
    return "mrb";
  case MarcRelator::MARKUP_EDITOR:
    return "mrk";
  case MarcRelator::MUSICAL_DIRECTOR:
    return "msd";
  case MarcRelator::METAL_ENGRAVER:
    return "mte";
  case MarcRelator::MINUTE_TAKER:
    return "mtk";
  case MarcRelator::MUSICIAN:
    return "mus";
  case MarcRelator::NARRATOR:
    return "nrt";
  case MarcRelator::OPPONENT:
    return "opn";
  case MarcRelator::ORIGINATOR:
    return "org";
  case MarcRelator::ORGANISER:
    return "orm";
  case MarcRelator::ONSCREEN_PRESENTER:
    return "osp";
  case MarcRelator::OTHER:
    return "oth";
  case MarcRelator::OWNER:
    return "own";
  case MarcRelator::PANELIST:
    return "pan";
  case MarcRelator::PATRON:
    return "pat";
  case MarcRelator::PUBLISHING_DIRECTOR:
    return "pbd";
  case MarcRelator::PUBLISHER:
    return "pbl";
  case MarcRelator::PROJECT_DIRECTOR:
    return "pdr";
  case MarcRelator::PROOFREADER:
    return "pfr";
  case MarcRelator::PHOTOGRAPHER:
    return "pht";
  case MarcRelator::PLATEMAKER:
    return "plt";
  case MarcRelator::PERMIITIN_AGENCY:
    return "pma";
  case MarcRelator::PRODUCTION_MANAGER:
    return "pmn";
  case MarcRelator::PRINTER_OF_PLATES:
    return "pop";
  case MarcRelator::PAPERMAKE:
    return "ppm";
  case MarcRelator::PUPPETEER:
    return "ppt";
  case MarcRelator::PRAESES:
    return "pra";
  case MarcRelator::PROCESS_CONTACT:
    return "prc";
  case MarcRelator::PRODUCTION_PERSONNAL:
    return "prd";
  case MarcRelator::PRESENTER:
    return "pre";
  case MarcRelator::PERFORMER:
    return "prf";
  case MarcRelator::PROGRAMMER:
    return "prg";
  case MarcRelator::PRINTMAKER:
    return "prm";
  case MarcRelator::PRODUCTION_COMPANY:
    return "prn";
  case MarcRelator::PRODUCER:
    return "pro";
  case MarcRelator::PRODUCTION_PLACE:
    return "prp";
  case MarcRelator::PRODUCTION_DESIGNER:
    return "prs";
  case MarcRelator::PRINTER:
    return "prt";
  case MarcRelator::PROVIDER:
    return "prv";
  case MarcRelator::PATENT_APPLICATION:
    return "pta";
  case MarcRelator::PLAINTIFF_APPELLEE:
    return "pte";
  case MarcRelator::PLAINTIFF:
    return "ptf";
  case MarcRelator::PATENT_HOLDER:
    return "pth";
  case MarcRelator::PLAINTIFF_APPELLANT:
    return "ptt";
  case MarcRelator::PUBLICATION_PLACE:
    return "pup";
  case MarcRelator::RUBRICATOR:
    return "rbr";
  case MarcRelator::RECORDIST:
    return "rcd";
  case MarcRelator::RECORDING_ENGINEER:
    return "rce";
  case MarcRelator::ADDRESSEE:
    return "rcp";
  case MarcRelator::RADIO_DIRECTOR:
    return "rdrelatord";
  case MarcRelator::REDAKTOR:
    return "red";
  case MarcRelator::RENDERER:
    return "ren";
  case MarcRelator::RESEARCHER:
    return "res";
  case MarcRelator::REVIEWER:
    return "rev";
  case MarcRelator::RADIO_PRODUCER:
    return "rpc";
  case MarcRelator::REPOSITORY:
    return "rps";
  case MarcRelator::REPOSRTER:
    return "rpt";
  case MarcRelator::RESPONSIBLE_PARTY:
    return "rpy";
  case MarcRelator::RESPONDANT_APPELLEE:
    return "rse";
  case MarcRelator::RESTAGER:
    return "rsg";
  case MarcRelator::RESPONDANT:
    return "rsp";
  case MarcRelator::RESTORATIONIST:
    return "rsr";
  case MarcRelator::RESPONDANT_APPELLANT:
    return "rst";
  case MarcRelator::RESEARCH_TEAM_HEAD:
    return "rth";
  case MarcRelator::RESEARCH_TEAM_MEMBER:
    return "rtm";
  case MarcRelator::SCIENTIFIC_ADVISOR:
    return "sad";
  case MarcRelator::SCENARIST:
    return "sce";
  case MarcRelator::SCULPTOR:
    return "scl";
  case MarcRelator::SCRIBE:
    return "scr";
  case MarcRelator::SOUND_DESIGNER:
    return "sds";
  case MarcRelator::SECRETARY:
    return "sec";
  case MarcRelator::STAGE_DIRECTOR:
    return "sgd";
  case MarcRelator::SIGNER:
    return "sgn";
  case MarcRelator::SUPPORTING_HOST:
    return "sht";
  case MarcRelator::SELLER:
    return "sll";
  case MarcRelator::SINGER:
    return "sng";
  case MarcRelator::SPEAKER:
    return "spk";
  case MarcRelator::SPONSOR:
    return "spn";
  case MarcRelator::SECOND_PARTY:
    return "spy";
  case MarcRelator::SURVEYOR:
    return "srv";
  case MarcRelator::SET_DESIGNER:
    return "std";
  case MarcRelator::SETTING:
    return "stg";
  case MarcRelator::STORYTELLER:
    return "stl";
  case MarcRelator::STAGE_MANAGER:
    return "stm";
  case MarcRelator::STANDARDS_BODY:
    return "stn";
  case MarcRelator::STEREOTYPER:
    return "str";
  case MarcRelator::TECHNICAL_DIRECTOR:
    return "tcd";
  case MarcRelator::TEACHER:
    return "tch";
  case MarcRelator::THESIS_ADVISOR:
    return "ths";
  case MarcRelator::TELEVISION_DIRECTOR:
    return "tld";
  case MarcRelator::TELEVISION_PRODUCER:
    return "tlp";
  case MarcRelator::TRANSCRIBER:
    return "trc";
  case MarcRelator::TRANSLATOR:
    return "trl";
  case MarcRelator::TYPE_DESIGNER:
    return "tyd";
  case MarcRelator::TYPOGRAPHER:
    return "tyg";
  case MarcRelator::UNIVERSITY_PLACE:
    return "uvp";
  case MarcRelator::VOICE_ACTOR:
    return "vac";
  case MarcRelator::VIDEOGRAPHER:
    return "vdg";
  case MarcRelator::VOCALIST:
    return "voc"; // Discontinued
  case MarcRelator::WRITER_OF_ADDED_COMMENTARY:
    return "wac";
  case MarcRelator::WRITER_OF_ADDED_LYRICS:
    return "wal";
  case MarcRelator::WRITER_OF_ACCOMPANYING_MATERIAL:
    return "wam";
  case MarcRelator::WRITER_OF_ADDED_TEXT:
    return "wat";
  case MarcRelator::WOODCUTTER:
    return "wdc";
  case MarcRelator::WOOD_ENGRAVER:
    return "wde";
  case MarcRelator::WRITER_OF_INTRODUCTION:
    return "win";
  case MarcRelator::WITNESS:
    return "wit";
  case MarcRelator::WRITER_OF_PREFACE:
    return "wpr";
  case MarcRelator::WRITER_OF_SUPPLEMENTARY_TEXTUAL_CONTENT:
    return "wst";
  default:
    return QString();
  }
}

MarcRelator MarcRelator::fromString(QString name)
{
  MarcRelator relator;
  if (name == "abr") {
    relator.setType(MarcRelator::ABRIDGER);
  } else if (name == "acp") {
    relator.setType(MarcRelator::ART_COPYIST);
  } else if (name == "act") {
    relator.setType(MarcRelator::ACTOR);
  } else if (name == "adi") {
    relator.setType(MarcRelator::ART_DIRECTOR);
  } else if (name == "adp") {
    relator.setType(MarcRelator::ADAPTER);
  } else if (name == "aft") {
    relator.setType(MarcRelator::AUTHOR_OF_AFTERWORD);
  } else if (name == "anl") {
    relator.setType(MarcRelator::ANALYST);
  } else if (name == "anm") {
    relator.setType(MarcRelator::ANIMATOR);
  } else if (name == "ann") {
    relator.setType(MarcRelator::ANNOTATOR);
  } else if (name == "ant") {
    relator.setType(MarcRelator::BIBLIOGRAPHIC_ANTECEDENT);
  } else if (name == "ape") {
    relator.setType(MarcRelator::APPELLEE);
  } else if (name == "apl") {
    relator.setType(MarcRelator::APPELLANT);
  } else if (name == "app") {
    relator.setType(MarcRelator::APPLICANT);
  } else if (name == "aqt") {
    relator.setType(MarcRelator::AUTHOR_IN_QUOTATIONS);
  } else if (name == "arc") {
    relator.setType(MarcRelator::ARCHITECT);
  } else if (name == "ard") {
    relator.setType(MarcRelator::ARTISTIC_DIRECTOR);
  } else if (name == "arr") {
    relator.setType(MarcRelator::ARRANGER);
  } else if (name == "art") {
    relator.setType(MarcRelator::ARTIST);
  } else if (name == "asg") {
    relator.setType(MarcRelator::ASSIGNEE);
  } else if (name == "asn") {
    relator.setType(MarcRelator::ASSOCIATED_NAME);
  } else if (name == "ato") {
    relator.setType(MarcRelator::AUTOGRAPHER);
  } else if (name == "att") {
    relator.setType(MarcRelator::ATTRIBUTED_NAME);
  } else if (name == "auc") {
    relator.setType(MarcRelator::AUCTIONEER);
  } else if (name == "aut") {
    relator.setType(MarcRelator::AUTHOR);
  } else if (name == "aud") {
    relator.setType(MarcRelator::AUTHOR_OF_DIALOG);
  } else if (name == "aui") {
    relator.setType(MarcRelator::AUTHOR_OF_INTRODUCTION);
  } else if (name == "aus") {
    relator.setType(MarcRelator::SCREENWRITER);
  } else if (name == "bdd") {
    relator.setType(MarcRelator::BINDING_DESIGNER);
  } else if (name == "bjd") {
    relator.setType(MarcRelator::BOOKJACKET_DESIGNER);
  } else if (name == "bkd") {
    relator.setType(MarcRelator::BOOK_DESIGNER);
  } else if (name == "bkp") {
    relator.setType(MarcRelator::BOOK_PRODUCER);
  } else if (name == "blw") {
    relator.setType(MarcRelator::BLURB_WRITER);
  } else if (name == "bnd") {
    relator.setType(MarcRelator::BINDER);
  } else if (name == "bpd") {
    relator.setType(MarcRelator::BOOKPLATE_DESIGNER);
  } else if (name == "brd") {
    relator.setType(MarcRelator::BROADCASTER);
  } else if (name == "brl") {
    relator.setType(MarcRelator::BRAILE_EMBOSSER);
  } else if (name == "bsl") {
    relator.setType(MarcRelator::BOOKSELLER);
  } else if (name == "cas") {
    relator.setType(MarcRelator::CASTER);
  } else if (name == "ccp") {
    relator.setType(MarcRelator::CONCEPTOR);
  } else if (name == "chr") {
    relator.setType(MarcRelator::CHOREOGRAPHER);
  } else if (name == "clb") {
    relator.setType(MarcRelator::COLLABORATOR); // Discontinued
  } else if (name == "cli") {
    relator.setType(MarcRelator::CLIENT);
  } else if (name == "cll") {
    relator.setType(MarcRelator::CALLIGRAPHER);
  } else if (name == "clr") {
    relator.setType(MarcRelator::COLORIST);
  } else if (name == "clt") {
    relator.setType(MarcRelator::COLLOTYPER);
  } else if (name == "cmm") {
    relator.setType(MarcRelator::COMMENTATOR);
  } else if (name == "cmp") {
    relator.setType(MarcRelator::COMPOSER);
  } else if (name == "cmt") {
    relator.setType(MarcRelator::COMPOSITOR);
  } else if (name == "cnd") {
    relator.setType(MarcRelator::CONDUCTOR);
  } else if (name == "cng") {
    relator.setType(MarcRelator::CINEMATOGRAPHER);
  } else if (name == "cns") {
    relator.setType(MarcRelator::CENSOR);
  } else if (name == "coe") {
    relator.setType(MarcRelator::CONTESTANT_APPELLEE);
  } else if (name == "col") {
    relator.setType(MarcRelator::CONTESTOR);
  } else if (name == "com") {
    relator.setType(MarcRelator::COMPILER);
  } else if (name == "con") {
    relator.setType(MarcRelator::CONSERVATOR);
  } else if (name == "cor") {
    relator.setType(MarcRelator::COLLECTION_REGISTRAR);
  } else if (name == "cos") {
    relator.setType(MarcRelator::CONTESTANT);
  } else if (name == "cot") {
    relator.setType(MarcRelator::CONTESTEE_APPELLANT);
  } else if (name == "cou") {
    relator.setType(MarcRelator::COURT_GOVERNED);
  } else if (name == "cov") {
    relator.setType(MarcRelator::COVER_DESIGNER);
  } else if (name == "cpc") {
    relator.setType(MarcRelator::COPYRIGHT_CLAIMANT);
  } else if (name == "cpe") {
    relator.setType(MarcRelator::COMPLAINANT_APPELLEE);
  } else if (name == "cph") {
    relator.setType(MarcRelator::COPYRIGHT_HOLDER);
  } else if (name == "cpl") {
    relator.setType(MarcRelator::COMPLAINANT);
  } else if (name == "cpt") {
    relator.setType(MarcRelator::COMPLAINANT_APPELLANT);
  } else if (name == "cre") {
    relator.setType(MarcRelator::CREATOR);
  } else if (name == "crp") {
    relator.setType(MarcRelator::CORRESPONDANT);
  } else if (name == "crr") {
    relator.setType(MarcRelator::CORRECTOR);
  } else if (name == "crt") {
    relator.setType(MarcRelator::COURT_REPORTER);
  } else if (name == "csl") {
    relator.setType(MarcRelator::CONSULTANT);
  } else if (name == "csp") {
    relator.setType(MarcRelator::CONSULTANT_TO_A_PROJECT);
  } else if (name == "cst") {
    relator.setType(MarcRelator::COSTUME_DESIGNER);
  } else if (name == "ctb") {
    relator.setType(MarcRelator::CONTRIBUTOR);
  } else if (name == "cte") {
    relator.setType(MarcRelator::CONTESTEE_APPELLEE);
  } else if (name == "ctg") {
    relator.setType(MarcRelator::CARTOGRAPHER);
  } else if (name == "ctr") {
    relator.setType(MarcRelator::CONTRACTOR);
  } else if (name == "cts") {
    relator.setType(MarcRelator::CONTESTEE);
  } else if (name == "ctt") {
    relator.setType(MarcRelator::CONTESTANT_APPELLANT);
  } else if (name == "cur") {
    relator.setType(MarcRelator::CURATOR);
  } else if (name == "cwt") {
    relator.setType(MarcRelator::COMMENTATOR_FOR_WRITTEN_TEXT);
  } else if (name == "dbp") {
    relator.setType(MarcRelator::DISTIBUTION_PLACE);
  } else if (name == "dfd") {
    relator.setType(MarcRelator::DEFENDANT);
  } else if (name == "dfe") {
    relator.setType(MarcRelator::DEFENDANT_APPELLEE);
  } else if (name == "dft") {
    relator.setType(MarcRelator::DEFENDANT_APPELLANT);
  } else if (name == "dgg") {
    relator.setType(MarcRelator::DEGREE_GRANTING_INSTITUTION);
  } else if (name == "dgs") {
    relator.setType(MarcRelator::DEGREE_SUPERVISOR);
  } else if (name == "dis") {
    relator.setType(MarcRelator::DISSERTANT);
  } else if (name == "dln") {
    relator.setType(MarcRelator::DELINEATOR);
  } else if (name == "dnc") {
    relator.setType(MarcRelator::DANCER);
  } else if (name == "dnr") {
    relator.setType(MarcRelator::DONOR);
  } else if (name == "dpc") {
    relator.setType(MarcRelator::DEPICTED);
  } else if (name == "dpt") {
    relator.setType(MarcRelator::DEPOSITOR);
  } else if (name == "drm") {
    relator.setType(MarcRelator::DRAFTSMAN);
  } else if (name == "drt") {
    relator.setType(MarcRelator::DIRECTOR);
  } else if (name == "dsr") {
    relator.setType(MarcRelator::DESIGNER);
  } else if (name == "dst") {
    relator.setType(MarcRelator::DISTRIBUTOR);
  } else if (name == "dtc") {
    relator.setType(MarcRelator::DATA_CONTRIBUTOR);
  } else if (name == "dte") {
    relator.setType(MarcRelator::DEDICATEE);
  } else if (name == "dtm") {
    relator.setType(MarcRelator::DATA_MANAGER);
  } else if (name == "dto") {
    relator.setType(MarcRelator::DEDICATOR);
  } else if (name == "dub") {
    relator.setType(MarcRelator::DUBIOUS_AUTHOR);
  } else if (name == "edc") {
    relator.setType(MarcRelator::EDITOR_OF_COMPILATION);
  } else if (name == "edm") {
    relator.setType(MarcRelator::EDITOR_OF_MOVING_IMAGE_WORK);
  } else if (name == "edt") {
    relator.setType(MarcRelator::EDITOR);
  } else if (name == "egr") {
    relator.setType(MarcRelator::ENGRAVER);
  } else if (name == "elg") {
    relator.setType(MarcRelator::ELECTRICIAN);
  } else if (name == "elt") {
    relator.setType(MarcRelator::ELECTROTYPER);
  } else if (name == "eng") {
    relator.setType(MarcRelator::ENGINEER);
  } else if (name == "enj") {
    relator.setType(MarcRelator::ENACTING_JURISTICTION);
  } else if (name == "etr") {
    relator.setType(MarcRelator::ETCHER);
  } else if (name == "evp") {
    relator.setType(MarcRelator::EVENT_PLACE);
  } else if (name == "exp") {
    relator.setType(MarcRelator::EXPERT);
  } else if (name == "fac") {
    relator.setType(MarcRelator::FACSIMILIST);
  } else if (name == "fds") {
    relator.setType(MarcRelator::FILM_DISTRIBUTOR);
  } else if (name == "fld") {
    relator.setType(MarcRelator::FIELD_DIRECTOR);
  } else if (name == "flm") {
    relator.setType(MarcRelator::FILM_EDITOR);
  } else if (name == "fmd") {
    relator.setType(MarcRelator::FILM_DIRECTOR);
  } else if (name == "fmk") {
    relator.setType(MarcRelator::FILM_MAKER);
  } else if (name == "fmo") {
    relator.setType(MarcRelator::FORMER_OWNOR);
  } else if (name == "fmp") {
    relator.setType(MarcRelator::FILM_PRODUCER);
  } else if (name == "fnd") {
    relator.setType(MarcRelator::FUNDER);
  } else if (name == "fpy") {
    relator.setType(MarcRelator::FIRST_PARTY);
  } else if (name == "frg") {
    relator.setType(MarcRelator::FORGER);
  } else if (name == "gis") {
    relator.setType(MarcRelator::GEOGRAPHIC_INFORMATION_SPECIALIST);
  } else if (name == "grt") {
    relator.setType(MarcRelator::GRAPHIC_TECHNICIAN); // Discontinued
  } else if (name == "his") {
    relator.setType(MarcRelator::HOST_INSTITUTION);
  } else if (name == "hnr") {
    relator.setType(MarcRelator::HONOREE);
  } else if (name == "hst") {
    relator.setType(MarcRelator::HOST);
  } else if (name == "ill") {
    relator.setType(MarcRelator::ILLISTRATOR);
  } else if (name == "ilu") {
    relator.setType(MarcRelator::ILLUMINATOR);
  } else if (name == "ins") {
    relator.setType(MarcRelator::INSCRIBER);
  } else if (name == "inv") {
    relator.setType(MarcRelator::INVENTOR);
  } else if (name == "isb") {
    relator.setType(MarcRelator::ISSUING_BODY);
  } else if (name == "itr") {
    relator.setType(MarcRelator::INSTRUMENTALIST);
  } else if (name == "ive") {
    relator.setType(MarcRelator::INTERVIEWEE);
  } else if (name == "ivr") {
    relator.setType(MarcRelator::INTERVIEWER);
  } else if (name == "jud") {
    relator.setType(MarcRelator::JUDGE);
  } else if (name == "jug") {
    relator.setType(MarcRelator::JURISTICTION_GOVERNED);
  } else if (name == "lbr") {
    relator.setType(MarcRelator::LABORATORY);
  } else if (name == "lbt") {
    relator.setType(MarcRelator::LIBRETTIST);
  } else if (name == "ldr") {
    relator.setType(MarcRelator::LABORATORY_DIRECTOR);
  } else if (name == "led") {
    relator.setType(MarcRelator::LEAD);
  } else if (name == "lee") {
    relator.setType(MarcRelator::LIBELLEE_APPELEE);
  } else if (name == "lel") {
    relator.setType(MarcRelator::LIBELLEE);
  } else if (name == "len") {
    relator.setType(MarcRelator::LENDER);
  } else if (name == "let") {
    relator.setType(MarcRelator::LIBELLEE_APPELLANT);
  } else if (name == "lgd") {
    relator.setType(MarcRelator::LIGHTING_DESIGNER);
  } else if (name == "lie") {
    relator.setType(MarcRelator::LIBELANT_APPELLEE);
  } else if (name == "lil") {
    relator.setType(MarcRelator::LIBELLANT);
  } else if (name == "lit") {
    relator.setType(MarcRelator::LIBELANT_APPELLANT);
  } else if (name == "lsa") {
    relator.setType(MarcRelator::LANDSCAPE_ARCHITECT);
  } else if (name == "lse") {
    relator.setType(MarcRelator::LICENSEE);
  } else if (name == "lso") {
    relator.setType(MarcRelator::LICENSOR);
  } else if (name == "ltg") {
    relator.setType(MarcRelator::LITHOGRAPHER);
  } else if (name == "lyr") {
    relator.setType(MarcRelator::LYRICIST);
  } else if (name == "mcp") {
    relator.setType(MarcRelator::MUSIC_COPYIST);
  } else if (name == "mdc") {
    relator.setType(MarcRelator::METADATA_CONTACT);
  } else if (name == "med") {
    relator.setType(MarcRelator::MEDIUM);
  } else if (name == "mfp") {
    relator.setType(MarcRelator::MANUFACTURE_PLACE);
  } else if (name == "mfr") {
    relator.setType(MarcRelator::MANFACTURER);
  } else if (name == "mod") {
    relator.setType(MarcRelator::MODERATOR);
  } else if (name == "mon") {
    relator.setType(MarcRelator::MONITOR);
  } else if (name == "mrb") {
    relator.setType(MarcRelator::MARBLER);
  } else if (name == "mrk") {
    relator.setType(MarcRelator::MARKUP_EDITOR);
  } else if (name == "msd") {
    relator.setType(MarcRelator::MUSICAL_DIRECTOR);
  } else if (name == "mte") {
    relator.setType(MarcRelator::METAL_ENGRAVER);
  } else if (name == "mtk") {
    relator.setType(MarcRelator::MINUTE_TAKER);
  } else if (name == "mus") {
    relator.setType(MarcRelator::MUSICIAN);
  } else if (name == "nrt") {
    relator.setType(MarcRelator::NARRATOR);
  } else if (name == "opn") {
    relator.setType(MarcRelator::OPPONENT);
  } else if (name == "org") {
    relator.setType(MarcRelator::ORIGINATOR);
  } else if (name == "orm") {
    relator.setType(MarcRelator::ORGANISER);
  } else if (name == "osp") {
    relator.setType(MarcRelator::ONSCREEN_PRESENTER);
  } else if (name.startsWith("oth")) {
    // starts with because non-standard relators are defined as
    // Other and start with 'oth.' followed by custon definition.
    relator.setType(MarcRelator::OTHER);
  } else if (name == "own") {
    relator.setType(MarcRelator::OWNER);
  } else if (name == "pan") {
    relator.setType(MarcRelator::PANELIST);
  } else if (name == "pat") {
    relator.setType(MarcRelator::PATRON);
  } else if (name == "pbd") {
    relator.setType(MarcRelator::PUBLISHING_DIRECTOR);
  } else if (name == "pbl") {
    relator.setType(MarcRelator::PUBLISHER);
  } else if (name == "pdr") {
    relator.setType(MarcRelator::PROJECT_DIRECTOR);
  } else if (name == "pfr") {
    relator.setType(MarcRelator::PROOFREADER);
  } else if (name == "pht") {
    relator.setType(MarcRelator::PHOTOGRAPHER);
  } else if (name == "plt") {
    relator.setType(MarcRelator::PLATEMAKER);
  } else if (name == "pma") {
    relator.setType(MarcRelator::PERMIITIN_AGENCY);
  } else if (name == "pmn") {
    relator.setType(MarcRelator::PRODUCTION_MANAGER);
  } else if (name == "pop") {
    relator.setType(MarcRelator::PRINTER_OF_PLATES);
  } else if (name == "ppm") {
    relator.setType(MarcRelator::PAPERMAKE);
  } else if (name == "ppt") {
    relator.setType(MarcRelator::PUPPETEER);
  } else if (name == "pra") {
    relator.setType(MarcRelator::PRAESES);
  } else if (name == "prc") {
    relator.setType(MarcRelator::PROCESS_CONTACT);
  } else if (name == "prd") {
    relator.setType(MarcRelator::PRODUCTION_PERSONNAL);
  } else if (name == "pre") {
    relator.setType(MarcRelator::PRESENTER);
  } else if (name == "prf") {
    relator.setType(MarcRelator::PERFORMER);
  } else if (name == "prg") {
    relator.setType(MarcRelator::PROGRAMMER);
  } else if (name == "prm") {
    relator.setType(MarcRelator::PRINTMAKER);
  } else if (name == "prn") {
    relator.setType(MarcRelator::PRODUCTION_COMPANY);
  } else if (name == "pro") {
    relator.setType(MarcRelator::PRODUCER);
  } else if (name == "prp") {
    relator.setType(MarcRelator::PRODUCTION_PLACE);
  } else if (name == "prs") {
    relator.setType(MarcRelator::PRODUCTION_DESIGNER);
  } else if (name == "prt") {
    relator.setType(MarcRelator::PRINTER);
  } else if (name == "prv") {
    relator.setType(MarcRelator::PROVIDER);
  } else if (name == "pta") {
    relator.setType(MarcRelator::PATENT_APPLICATION);
  } else if (name == "pte") {
    relator.setType(MarcRelator::PLAINTIFF_APPELLEE);
  } else if (name == "ptf") {
    relator.setType(MarcRelator::PLAINTIFF);
  } else if (name == "pth") {
    relator.setType(MarcRelator::PATENT_HOLDER);
  } else if (name == "ptt") {
    relator.setType(MarcRelator::PLAINTIFF_APPELLANT);
  } else if (name == "pup") {
    relator.setType(MarcRelator::PUBLICATION_PLACE);
  } else if (name == "rbr") {
    relator.setType(MarcRelator::RUBRICATOR);
  } else if (name == "rcd") {
    relator.setType(MarcRelator::RECORDIST);
  } else if (name == "rce") {
    relator.setType(MarcRelator::RECORDING_ENGINEER);
  } else if (name == "rcp") {
    relator.setType(MarcRelator::ADDRESSEE);
  } else if (name == "rdd") {
    relator.setType(MarcRelator::RADIO_DIRECTOR);
  } else if (name == "red") {
    relator.setType(MarcRelator::REDAKTOR);
  } else if (name == "ren") {
    relator.setType(MarcRelator::RENDERER);
  } else if (name == "res") {
    relator.setType(MarcRelator::RESEARCHER);
  } else if (name == "rev") {
    relator.setType(MarcRelator::REVIEWER);
  } else if (name == "rpc") {
    relator.setType(MarcRelator::RADIO_PRODUCER);
  } else if (name == "rps") {
    relator.setType(MarcRelator::REPOSITORY);
  } else if (name == "rpt") {
    relator.setType(MarcRelator::REPOSRTER);
  } else if (name == "rpy") {
    relator.setType(MarcRelator::RESPONSIBLE_PARTY);
  } else if (name == "rse") {
    relator.setType(MarcRelator::RESPONDANT_APPELLEE);
  } else if (name == "rsg") {
    relator.setType(MarcRelator::RESTAGER);
  } else if (name == "rsp") {
    relator.setType(MarcRelator::RESPONDANT);
  } else if (name == "rsr") {
    relator.setType(MarcRelator::RESTORATIONIST);
  } else if (name == "rst") {
    relator.setType(MarcRelator::RESPONDANT_APPELLANT);
  } else if (name == "rth") {
    relator.setType(MarcRelator::RESEARCH_TEAM_HEAD);
  } else if (name == "rtm") {
    relator.setType(MarcRelator::RESEARCH_TEAM_MEMBER);
  } else if (name == "sad") {
    relator.setType(MarcRelator::SCIENTIFIC_ADVISOR);
  } else if (name == "sce") {
    relator.setType(MarcRelator::SCENARIST);
  } else if (name == "scl") {
    relator.setType(MarcRelator::SCULPTOR);
  } else if (name == "scr") {
    relator.setType(MarcRelator::SCRIBE);
  } else if (name == "sds") {
    relator.setType(MarcRelator::SOUND_DESIGNER);
  } else if (name == "sec") {
    relator.setType(MarcRelator::SECRETARY);
  } else if (name == "sgd") {
    relator.setType(MarcRelator::STAGE_DIRECTOR);
  } else if (name == "sgn") {
    relator.setType(MarcRelator::SIGNER);
  } else if (name == "sht") {
    relator.setType(MarcRelator::SUPPORTING_HOST);
  } else if (name == "sll") {
    relator.setType(MarcRelator::SELLER);
  } else if (name == "sng") {
    relator.setType(MarcRelator::SINGER);
  } else if (name == "spk") {
    relator.setType(MarcRelator::SPEAKER);
  } else if (name == "spn") {
    relator.setType(MarcRelator::SPONSOR);
  } else if (name == "spy") {
    relator.setType(MarcRelator::SECOND_PARTY);
  } else if (name == "srv") {
    relator.setType(MarcRelator::SURVEYOR);
  } else if (name == "std") {
    relator.setType(MarcRelator::SET_DESIGNER);
  } else if (name == "stg") {
    relator.setType(MarcRelator::SETTING);
  } else if (name == "stl") {
    relator.setType(MarcRelator::STORYTELLER);
  } else if (name == "stm") {
    relator.setType(MarcRelator::STAGE_MANAGER);
  } else if (name == "stn") {
    relator.setType(MarcRelator::STANDARDS_BODY);
  } else if (name == "str") {
    relator.setType(MarcRelator::STEREOTYPER);
  } else if (name == "tcd") {
    relator.setType(MarcRelator::TECHNICAL_DIRECTOR);
  } else if (name == "tch") {
    relator.setType(MarcRelator::TEACHER);
  } else if (name == "ths") {
    relator.setType(MarcRelator::THESIS_ADVISOR);
  } else if (name == "tld") {
    relator.setType(MarcRelator::TELEVISION_DIRECTOR);
  } else if (name == "tlp") {
    relator.setType(MarcRelator::TELEVISION_PRODUCER);
  } else if (name == "trc") {
    relator.setType(MarcRelator::TRANSCRIBER);
  } else if (name == "trl") {
    relator.setType(MarcRelator::TRANSLATOR);
  } else if (name == "tyd") {
    relator.setType(MarcRelator::TYPE_DESIGNER);
  } else if (name == "tyg") {
    relator.setType(MarcRelator::TYPOGRAPHER);
  } else if (name == "uvp") {
    relator.setType(MarcRelator::UNIVERSITY_PLACE);
  } else if (name == "vac") {
    relator.setType(MarcRelator::VOICE_ACTOR);
  } else if (name == "vdg") {
    relator.setType(MarcRelator::VIDEOGRAPHER);
  } else if (name == "voc") {
    relator.setType(MarcRelator::VOCALIST); // Discontinued
  } else if (name == "wac") {
    relator.setType(MarcRelator::WRITER_OF_ADDED_COMMENTARY);
  } else if (name == "wal") {
    relator.setType(MarcRelator::WRITER_OF_ADDED_LYRICS);
  } else if (name == "wam") {
    relator.setType(MarcRelator::WRITER_OF_ACCOMPANYING_MATERIAL);
  } else if (name == "wat") {
    relator.setType(MarcRelator::WRITER_OF_ADDED_TEXT);
  } else if (name == "wdc") {
    relator.setType(MarcRelator::WOODCUTTER);
  } else if (name == "wde") {
    relator.setType(MarcRelator::WOOD_ENGRAVER);
  } else if (name == "win") {
    relator.setType(MarcRelator::WRITER_OF_INTRODUCTION);
  } else if (name == "wit") {
    relator.setType(MarcRelator::WITNESS);
  } else if (name == "wpr") {
    relator.setType(MarcRelator::WRITER_OF_PREFACE);
  } else if (name == "wst") {
    relator.setType(MarcRelator::WRITER_OF_SUPPLEMENTARY_TEXTUAL_CONTENT);
  } else {
    relator.setType(MarcRelator::NO_TYPE);
  }
  if (relator.type() != NO_TYPE) {
    relator.setCode(name);
  } else {
    relator.setCode("");
  }
  return relator;
}

bool MarcRelator::isRelator(QString name)
{
  MarcRelator relator = fromString(name);
  if (relator.type() != NO_TYPE)
    return true;
  return false;
}
