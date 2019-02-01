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
  case ABRIDGER:
    return "abr";
  case ART_COPYIST:
    return "acp";
  case ACTOR:
    return "act";
  case ART_DIRECTOR:
    return "adi";
  case ADAPTER:
    return "adp";
  case AUTHOR_OF_AFTERWORD:
    return "aft";
  case ANALYST:
    return "anl";
  case ANIMATOR:
    return "anm";
  case ANNOTATOR:
    return "ann";
  case BIBLIOGRAPHIC_ANTECEDENT:
    return "ant";
  case APPELLEE:
    return "ape";
  case APPELLANT:
    return "apl";
  case APPLICANT:
    return "app";
  case AUTHOR_IN_QUOTATIONS:
    return "aqt";
  case ARCHITECT:
    return "arc";
  case ARTISTIC_DIRECTOR:
    return "ard";
  case ARRANGER:
    return "arr";
  case ARTIST:
    return "art";
  case ASSIGNEE:
    return "asg";
  case ASSOCIATED_NAME:
    return "asn";
  case AUTOGRAPHER:
    return "ato";
  case ATTRIBUTED_NAME:
    return "att";
  case AUCTIONEER:
    return "auc";
  case AUTHOR:
    return "aut";
  case AUTHOR_OF_DIALOG:
    return "aud";
  case AUTHOR_OF_INTRODUCTION:
    return "aui";
  case SCREENWRITER:
    return "aus";
  case BINDING_DESIGNER:
    return "bdd";
  case BOOKJACKET_DESIGNER:
    return "bjd";
  case BOOK_DESIGNER:
    return "bkd";
  case BOOK_PRODUCER:
    return "bkp";
  case BLURB_WRITER:
    return "blw";
  case BINDER:
    return "bnd";
  case BOOKPLATE_DESIGNER:
    return "bpd";
  case BROADCASTER:
    return "brd";
  case BRAILE_EMBOSSER:
    return "brl";
  case BOOKSELLER:
    return "bsl";
  case CASTER:
    return "cas";
  case CONCEPTOR:
    return "ccp";
  case CHOREOGRAPHER:
    return "chr";
  case COLLABORATOR:
    return "clb"; // Discontinued
  case CLIENT:
    return "cli";
  case CALLIGRAPHER:
    return "cll";
  case COLORIST:
    return "clr";
  case COLLOTYPER:
    return "clt";
  case COMMENTATOR:
    return "cmm";
  case COMPOSER:
    return "cmp";
  case COMPOSITOR:
    return "cmt";
  case CONDUCTOR:
    return "cnd";
  case CINEMATOGRAPHER:
    return "cng";
  case CENSOR:
    return "cns";
  case CONTESTANT_APPELLEE:
    return "coe";
  case CONTESTOR:
    return "col";
  case COMPILER:
    return "com";
  case CONSERVATOR:
    return "con";
  case COLLECTION_REGISTRAR:
    return "cor";
  case CONTESTANT:
    return "cos";
  case CONTESTEE_APPELLANT:
    return "cot";
  case COURT_GOVERNED:
    return "cou";
  case COVER_DESIGNER:
    return "cov";
  case COPYRIGHT_CLAIMANT:
    return "cpc";
  case COMPLAINANT_APPELLEE:
    return "cpe";
  case COPYRIGHT_HOLDER:
    return "cph";
  case COMPLAINANT:
    return "cpl";
  case COMPLAINANT_APPELLANT:
    return "cpt";
  case CREATOR:
    return "cre";
  case CORRESPONDANT:
    return "crp";
  case CORRECTOR:
    return "crr";
  case COURT_REPORTER:
    return "crt";
  case CONSULTANT:
    return "csl";
  case CONSULTANT_TO_A_PROJECT:
    return "csp";
  case COSTUME_DESIGNER:
    return "cst";
  case CONTRIBUTOR:
    return "ctb";
  case CONTESTEE_APPELLEE:
    return "cte";
  case CARTOGRAPHER:
    return "ctg";
  case CONTRACTOR:
    return "ctr";
  case CONTESTEE:
    return "cts";
  case CONTESTANT_APPELLANT:
    return "ctt";
  case CURATOR:
    return "cur";
  case COMMENTATOR_FOR_WRITTEN_TEXT:
    return "cwt";
  case DISTIBUTION_PLACE:
    return "dbp";
  case DEFENDANT:
    return "dfd";
  case DEFENDANT_APPELLEE:
    return "dfe";
  case DEFENDANT_APPELLANT:
    return "dft";
  case DEGREE_GRANTING_INSTITUTION:
    return "dgg";
  case DEGREE_SUPERVISOR:
    return "dgs";
  case DISSERTANT:
    return "dis";
  case DELINEATOR:
    return "dln";
  case DANCER:
    return "dnc";
  case DONOR:
    return "dnr";
  case DEPICTED:
    return "dpc";
  case DEPOSITOR:
    return "dpt";
  case DRAFTSMAN:
    return "drm";
  case DIRECTOR:
    return "drt";
  case DESIGNER:
    return "dsr";
  case DISTRIBUTOR:
    return "dst";
  case DATA_CONTRIBUTOR:
    return "dtc";
  case DEDICATEE:
    return "dte";
  case DATA_MANAGER:
    return "dtm";
  case DEDICATOR:
    return "dto";
  case DUBIOUS_AUTHOR:
    return "dub";
  case EDITOR_OF_COMPILATION:
    return "edc";
  case EDITOR_OF_MOVING_IMAGE_WORK:
    return "edm";
  case EDITOR:
    return "edt";
  case ENGRAVER:
    return "egr";
  case ELECTRICIAN:
    return "elg";
  case ELECTROTYPER:
    return "elt";
  case ENGINEER:
    return "eng";
  case ENACTING_JURISTICTION:
    return "enj";
  case ETCHER:
    return "etr";
  case EVENT_PLACE:
    return "evp";
  case EXPERT:
    return "exp";
  case FACSIMILIST:
    return "fac";
  case FILM_DISTRIBUTOR:
    return "fds";
  case FIELD_DIRECTOR:
    return "fld";
  case FILM_EDITOR:
    return "flm";
  case FILM_DIRECTOR:
    return "fmd";
  case FILM_MAKER:
    return "fmk";
  case FORMER_OWNOR:
    return "fmo";
  case FILM_PRODUCER:
    return "fmp";
  case FUNDER:
    return "fnd";
  case FIRST_PARTY:
    return "fpy";
  case FORGER:
    return "frg";
  case GEOGRAPHIC_INFORMATION_SPECIALIST:
    return "gis";
  case GRAPHIC_TECHNICIAN:
    return "grt"; // Discontinued
  case HOST_INSTITUTION:
    return "his";
  case HONOREE:
    return "hnr";
  case HOST:
    return "hst";
  case ILLISTRATOR:
    return "ill";
  case ILLUMINATOR:
    return "ilu";
  case INSCRIBER:
    return "ins";
  case INVENTOR:
    return "inv";
  case ISSUING_BODY:
    return "isb";
  case INSTRUMENTALIST:
    return "itr";
  case INTERVIEWEE:
    return "ive";
  case INTERVIEWER:
    return "ivr";
  case JUDGE:
    return "jud";
  case JURISTICTION_GOVERNED:
    return "jug";
  case LABORATORY:
    return "lbr";
  case LIBRETTIST:
    return "lbt";
  case LABORATORY_DIRECTOR:
    return "ldr";
  case LEAD:
    return "led";
  case LIBELLEE_APPELEE:
    return "lee";
  case LIBELLEE:
    return "lel";
  case LENDER:
    return "len";
  case LIBELLEE_APPELLANT:
    return "let";
  case LIGHTING_DESIGNER:
    return "lgd";
  case LIBELANT_APPELLEE:
    return "lie";
  case LIBELLANT:
    return "lil";
  case LIBELANT_APPELLANT:
    return "lit";
  case LANDSCAPE_ARCHITECT:
    return "lsa";
  case LICENSEE:
    return "lse";
  case LICENSOR:
    return "lso";
  case LITHOGRAPHER:
    return "ltg";
  case LYRICIST:
    return "lyr";
  case MUSIC_COPYIST:
    return "mcp";
  case METADATA_CONTACT:
    return "mdc";
  case MEDIUM:
    return "med";
  case MANUFACTURE_PLACE:
    return "mfp";
  case MANFACTURER:
    return "mfr";
  case MODERATOR:
    return "mod";
  case MONITOR:
    return "mon";
  case MARBLER:
    return "mrb";
  case MARKUP_EDITOR:
    return "mrk";
  case MUSICAL_DIRECTOR:
    return "msd";
  case METAL_ENGRAVER:
    return "mte";
  case MINUTE_TAKER:
    return "mtk";
  case MUSICIAN:
    return "mus";
  case NARRATOR:
    return "nrt";
  case OPPONENT:
    return "opn";
  case ORIGINATOR:
    return "org";
  case ORGANISER:
    return "orm";
  case ONSCREEN_PRESENTER:
    return "osp";
  case OTHER:
    return "oth";
  case OWNER:
    return "own";
  case PANELIST:
    return "pan";
  case PATRON:
    return "pat";
  case PUBLISHING_DIRECTOR:
    return "pbd";
  case PUBLISHER:
    return "pbl";
  case PROJECT_DIRECTOR:
    return "pdr";
  case PROOFREADER:
    return "pfr";
  case PHOTOGRAPHER:
    return "pht";
  case PLATEMAKER:
    return "plt";
  case PERMIITIN_AGENCY:
    return "pma";
  case PRODUCTION_MANAGER:
    return "pmn";
  case PRINTER_OF_PLATES:
    return "pop";
  case PAPERMAKE:
    return "ppm";
  case PUPPETEER:
    return "ppt";
  case PRAESES:
    return "pra";
  case PROCESS_CONTACT:
    return "prc";
  case PRODUCTION_PERSONNAL:
    return "prd";
  case PRESENTER:
    return "pre";
  case PERFORMER:
    return "prf";
  case PROGRAMMER:
    return "prg";
  case PRINTMAKER:
    return "prm";
  case PRODUCTION_COMPANY:
    return "prn";
  case PRODUCER:
    return "pro";
  case PRODUCTION_PLACE:
    return "prp";
  case PRODUCTION_DESIGNER:
    return "prs";
  case PRINTER:
    return "prt";
  case PROVIDER:
    return "prv";
  case PATENT_APPLICATION:
    return "pta";
  case PLAINTIFF_APPELLEE:
    return "pte";
  case Plaintiff:
    return "ptf";
  case PatentHolder:
    return "pth";
  case PlaintiffAppellant:
    return "ptt";
  case PublicationPlace:
    return "pup";
  case Rubricator:
    return "rbr";
  case Recordist:
    return "rcd";
  case RecordingEngineer:
    return "rce";
  case Addressee:
    return "rcp";
  case RadioDirector:
    return "rdrelatord";
  case Redaktor:
    return "red";
  case Renderer:
    return "ren";
  case Researcher:
    return "res";
  case Reviewer:
    return "rev";
  case RadioProducer:
    return "rpc";
  case Repository:
    return "rps";
  case Reporter:
    return "rpt";
  case ResponsibleParty:
    return "rpy";
  case RespondentAppellee:
    return "rse";
  case Restager:
    return "rsg";
  case Respondent:
    return "rsp";
  case Restorationist:
    return "rsr";
  case RespondentAppellant:
    return "rst";
  case ResearchTeamHead:
    return "rth";
  case ResearchTeamMember:
    return "rtm";
  case ScientificAdvisor:
    return "sad";
  case Scenarist:
    return "sce";
  case Sculptor:
    return "scl";
  case Scribe:
    return "scr";
  case SoundDesigner:
    return "sds";
  case Secretary:
    return "sec";
  case StageDirector:
    return "sgd";
  case Signer:
    return "sgn";
  case SupportingHost:
    return "sht";
  case Seller:
    return "sll";
  case Singer:
    return "sng";
  case Speaker:
    return "spk";
  case Sponsor:
    return "spn";
  case SecondParty:
    return "spy";
  case Surveyor:
    return "srv";
  case SetDesigner:
    return "std";
  case Setting:
    return "stg";
  case Storyteller:
    return "stl";
  case StageManager:
    return "stm";
  case StandardsBody:
    return "stn";
  case Stereotyper:
    return "str";
  case TechnicalDirector:
    return "tcd";
  case Teacher:
    return "tch";
  case ThesisAdvisor:
    return "ths";
  case TelevisionDirector:
    return "tld";
  case TelevisionProducer:
    return "tlp";
  case Transcriber:
    return "trc";
  case Translator:
    return "trl";
  case TypeDesigner:
    return "tyd";
  case Typographer:
    return "tyg";
  case UniversityPlace:
    return "uvp";
  case VoiceActor:
    return "vac";
  case Videographer:
    return "vdg";
  case Vocalist:
    return "voc"; // Discontinued
  case WriterOfAddedCommentary:
    return "wac";
  case WriterOfAddedLyrics:
    return "wal";
  case WriterOfAccompanyingMaterial:
    return "wam";
  case WriterOfAddedText:
    return "wat";
  case Woodcutter:
    return "wdc";
  case WoodEngraver:
    return "wde";
  case WriterOfIntroduction:
    return "win";
  case Witness:
    return "wit";
  case WriterOfPreface:
    return "wpr";
  case WriterOfSupplementaryTextualContent:
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
    relator.setType(MarcRelator::Plaintiff);
  } else if (name == "pth") {
    relator.setType(MarcRelator::PatentHolder);
  } else if (name == "ptt") {
    relator.setType(MarcRelator::PlaintiffAppellant);
  } else if (name == "pup") {
    relator.setType(MarcRelator::PublicationPlace);
  } else if (name == "rbr") {
    relator.setType(MarcRelator::Rubricator);
  } else if (name == "rcd") {
    relator.setType(MarcRelator::Recordist);
  } else if (name == "rce") {
    relator.setType(MarcRelator::RecordingEngineer);
  } else if (name == "rcp") {
    relator.setType(MarcRelator::Addressee);
  } else if (name == "rdd") {
    relator.setType(MarcRelator::RadioDirector);
  } else if (name == "red") {
    relator.setType(MarcRelator::Redaktor);
  } else if (name == "ren") {
    relator.setType(MarcRelator::Renderer);
  } else if (name == "res") {
    relator.setType(MarcRelator::Researcher);
  } else if (name == "rev") {
    relator.setType(MarcRelator::Reviewer);
  } else if (name == "rpc") {
    relator.setType(MarcRelator::RadioProducer);
  } else if (name == "rps") {
    relator.setType(MarcRelator::Repository);
  } else if (name == "rpt") {
    relator.setType(MarcRelator::Reporter);
  } else if (name == "rpy") {
    relator.setType(MarcRelator::ResponsibleParty);
  } else if (name == "rse") {
    relator.setType(MarcRelator::RespondentAppellee);
  } else if (name == "rsg") {
    relator.setType(MarcRelator::Restager);
  } else if (name == "rsp") {
    relator.setType(MarcRelator::Respondent);
  } else if (name == "rsr") {
    relator.setType(MarcRelator::Restorationist);
  } else if (name == "rst") {
    relator.setType(MarcRelator::RespondentAppellant);
  } else if (name == "rth") {
    relator.setType(MarcRelator::ResearchTeamHead);
  } else if (name == "rtm") {
    relator.setType(MarcRelator::ResearchTeamMember);
  } else if (name == "sad") {
    relator.setType(MarcRelator::ScientificAdvisor);
  } else if (name == "sce") {
    relator.setType(MarcRelator::Scenarist);
  } else if (name == "scl") {
    relator.setType(MarcRelator::Sculptor);
  } else if (name == "scr") {
    relator.setType(MarcRelator::Scribe);
  } else if (name == "sds") {
    relator.setType(MarcRelator::SoundDesigner);
  } else if (name == "sec") {
    relator.setType(MarcRelator::Secretary);
  } else if (name == "sgd") {
    relator.setType(MarcRelator::StageDirector);
  } else if (name == "sgn") {
    relator.setType(MarcRelator::Signer);
  } else if (name == "sht") {
    relator.setType(MarcRelator::SupportingHost);
  } else if (name == "sll") {
    relator.setType(MarcRelator::Seller);
  } else if (name == "sng") {
    relator.setType(MarcRelator::Singer);
  } else if (name == "spk") {
    relator.setType(MarcRelator::Speaker);
  } else if (name == "spn") {
    relator.setType(MarcRelator::Sponsor);
  } else if (name == "spy") {
    relator.setType(MarcRelator::SecondParty);
  } else if (name == "srv") {
    relator.setType(MarcRelator::Surveyor);
  } else if (name == "std") {
    relator.setType(MarcRelator::SetDesigner);
  } else if (name == "stg") {
    relator.setType(MarcRelator::Setting);
  } else if (name == "stl") {
    relator.setType(MarcRelator::Storyteller);
  } else if (name == "stm") {
    relator.setType(MarcRelator::StageManager);
  } else if (name == "stn") {
    relator.setType(MarcRelator::StandardsBody);
  } else if (name == "str") {
    relator.setType(MarcRelator::Stereotyper);
  } else if (name == "tcd") {
    relator.setType(MarcRelator::TechnicalDirector);
  } else if (name == "tch") {
    relator.setType(MarcRelator::Teacher);
  } else if (name == "ths") {
    relator.setType(MarcRelator::ThesisAdvisor);
  } else if (name == "tld") {
    relator.setType(MarcRelator::TelevisionDirector);
  } else if (name == "tlp") {
    relator.setType(MarcRelator::TelevisionProducer);
  } else if (name == "trc") {
    relator.setType(MarcRelator::Transcriber);
  } else if (name == "trl") {
    relator.setType(MarcRelator::Translator);
  } else if (name == "tyd") {
    relator.setType(MarcRelator::TypeDesigner);
  } else if (name == "tyg") {
    relator.setType(MarcRelator::Typographer);
  } else if (name == "uvp") {
    relator.setType(MarcRelator::UniversityPlace);
  } else if (name == "vac") {
    relator.setType(MarcRelator::VoiceActor);
  } else if (name == "vdg") {
    relator.setType(MarcRelator::Videographer);
  } else if (name == "voc") {
    relator.setType(MarcRelator::Vocalist); // Discontinued
  } else if (name == "wac") {
    relator.setType(MarcRelator::WriterOfAddedCommentary);
  } else if (name == "wal") {
    relator.setType(MarcRelator::WriterOfAddedLyrics);
  } else if (name == "wam") {
    relator.setType(MarcRelator::WriterOfAccompanyingMaterial);
  } else if (name == "wat") {
    relator.setType(MarcRelator::WriterOfAddedText);
  } else if (name == "wdc") {
    relator.setType(MarcRelator::Woodcutter);
  } else if (name == "wde") {
    relator.setType(MarcRelator::WoodEngraver);
  } else if (name == "win") {
    relator.setType(MarcRelator::WriterOfIntroduction);
  } else if (name == "wit") {
    relator.setType(MarcRelator::Witness);
  } else if (name == "wpr") {
    relator.setType(MarcRelator::WriterOfPreface);
  } else if (name == "wst") {
    relator.setType(MarcRelator::WriterOfSupplementaryTextualContent);
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
