#include "marcrelator.h"


MarcRelator::MarcRelator()
{
  m_type = no_type;
}

MarcRelator::Relator MarcRelator::type()
{
  return m_type;
}

void MarcRelator::setType(MarcRelator::Relator type)
{
  m_type = type;
}

QString MarcRelator::asString()
{
  return MarcRelator::toString(m_type);
}

QString MarcRelator::toString(MarcRelator::Relator relator)
{
  switch (relator) {
  case Abridger:
    return "abr";
  case ArtCopyist:
    return "acp";
  case Actor:
    return "act";
  case ArtDirector:
    return "adi";
  case Adapter:
    return "adp";
  case AuthorOfAfterword:
    return "aft";
  case Analyst:
    return "anl";
  case Animator:
    return "anm";
  case Annotator:
    return "ann";
  case BibliographicAntecedent:
    return "ant";
  case Appellee:
    return "ape";
  case Appellant:
    return "apl";
  case Applicant:
    return "app";
  case AuthorInQuotations:
    return "aqt";
  case Architect:
    return "arc";
  case ArtisticDirector:
    return "ard";
  case Arranger:
    return "arr";
  case Artist:
    return "art";
  case Assignee:
    return "asg";
  case AssociatedName:
    return "asn";
  case Autographer:
    return "ato";
  case AttributedName:
    return "att";
  case Auctioneer:
    return "auc";
  case Author:
    return "aut";
  case AuthorOfDialog:
    return "aud";
  case AuthorOfIntroduction:
    return "aui";
  case Screenwriter:
    return "aus";
  case BindingDesigner:
    return "bdd";
  case BookjacketDesigner:
    return "bjd";
  case BookDesigner:
    return "bkd";
  case BookProducer:
    return "bkp";
  case BlurbWriter:
    return "blw";
  case Binder:
    return "bnd";
  case BookplateDesigner:
    return "bpd";
  case Broadcaster:
    return "brd";
  case BrailleEmbosser:
    return "brl";
  case Bookseller:
    return "bsl";
  case Caster:
    return "cas";
  case Conceptor:
    return "ccp";
  case Choreographer:
    return "chr";
  case Collaborator:
    return "clb"; // Discontinued
  case Client:
    return "cli";
  case Calligrapher:
    return "cll";
  case Colorist:
    return "clr";
  case Collotyper:
    return "clt";
  case Commentator:
    return "cmm";
  case Composer:
    return "cmp";
  case Compositor:
    return "cmt";
  case Conductor:
    return "cnd";
  case Cinematographer:
    return "cng";
  case Censor:
    return "cns";
  case ContestantAppellee:
    return "coe";
  case Collector:
    return "col";
  case Compiler:
    return "com";
  case Conservator:
    return "con";
  case CollectionRegistrar:
    return "cor";
  case Contestant:
    return "cos";
  case ContesteeAppellant:
    return "cot";
  case CourtGoverned:
    return "cou";
  case CoverDesigner:
    return "cov";
  case CopyrightClaimant:
    return "cpc";
  case ComplainantAppellee:
    return "cpe";
  case CopyrightHolder:
    return "cph";
  case Complainant:
    return "cpl";
  case ComplainantAppellant:
    return "cpt";
  case Creator:
    return "cre";
  case Correspondent:
    return "crp";
  case Corrector:
    return "crr";
  case CourtReporter:
    return "crt";
  case Consultant:
    return "csl";
  case ConsultantToAProject:
    return "csp";
  case CostumeDesigner:
    return "cst";
  case Contributor:
    return "ctb";
  case ContesteeAppellee:
    return "cte";
  case Cartographer:
    return "ctg";
  case Contractor:
    return "ctr";
  case Contestee:
    return "cts";
  case ContestantAppellant:
    return "ctt";
  case Curator:
    return "cur";
  case CommentatorForWrittenText:
    return "cwt";
  case DistributionPlace:
    return "dbp";
  case Defendant:
    return "dfd";
  case DefendantAppellee:
    return "dfe";
  case DefendantAppellant:
    return "dft";
  case DegreeGrantingInstitution:
    return "dgg";
  case DegreeSupervisor:
    return "dgs";
  case Dissertant:
    return "dis";
  case Delineator:
    return "dln";
  case Dancer:
    return "dnc";
  case Donor:
    return "dnr";
  case Depicted:
    return "dpc";
  case Depositor:
    return "dpt";
  case Draftsman:
    return "drm";
  case Director:
    return "drt";
  case Designer:
    return "dsr";
  case Distributor:
    return "dst";
  case DataContributor:
    return "dtc";
  case Dedicatee:
    return "dte";
  case DataManager:
    return "dtm";
  case Dedicator:
    return "dto";
  case DubiousAuthor:
    return "dub";
  case EditorOfCompilation:
    return "edc";
  case EditorOfMovingImageWork:
    return "edm";
  case Editor:
    return "edt";
  case Engraver:
    return "egr";
  case Electrician:
    return "elg";
  case Electrotyper:
    return "elt";
  case Engineer:
    return "eng";
  case EnactingJurisdiction:
    return "enj";
  case Etcher:
    return "etr";
  case EventPlace:
    return "evp";
  case Expert:
    return "exp";
  case Facsimilist:
    return "fac";
  case FilmDistributor:
    return "fds";
  case FieldDirector:
    return "fld";
  case FilmEditor:
    return "flm";
  case FilmDirector:
    return "fmd";
  case Filmmaker:
    return "fmk";
  case FormerOwner:
    return "fmo";
  case FilmProducer:
    return "fmp";
  case Funder:
    return "fnd";
  case FirstParty:
    return "fpy";
  case Forger:
    return "frg";
  case GeographicInformationSpecialist:
    return "gis";
  case GraphicTechnician:
    return "grt"; // Discontinued
  case HostInstitution:
    return "his";
  case Honoree:
    return "hnr";
  case Host:
    return "hst";
  case Illustrator:
    return "ill";
  case Illuminator:
    return "ilu";
  case Inscriber:
    return "ins";
  case Inventor:
    return "inv";
  case IssuingBody:
    return "isb";
  case Instrumentalist:
    return "itr";
  case Interviewee:
    return "ive";
  case Interviewer:
    return "ivr";
  case Judge:
    return "jud";
  case JurisdictionGoverned:
    return "jug";
  case Laboratory:
    return "lbr";
  case Librettist:
    return "lbt";
  case LaboratoryDirector:
    return "ldr";
  case Lead:
    return "led";
  case LibeleeAppellee:
    return "lee";
  case Libelee:
    return "lel";
  case Lender:
    return "len";
  case LibeleeAppellant:
    return "let";
  case LightingDesigner:
    return "lgd";
  case LibelantAppellee:
    return "lie";
  case Libelant:
    return "lil";
  case LibelantAppellant:
    return "lit";
  case LandscapeArchitect:
    return "lsa";
  case Licensee:
    return "lse";
  case Licensor:
    return "lso";
  case Lithographer:
    return "ltg";
  case Lyricist:
    return "lyr";
  case MusicCopyist:
    return "mcp";
  case MetadataContact:
    return "mdc";
  case Medium:
    return "med";
  case ManufacturePlace:
    return "mfp";
  case Manufacturer:
    return "mfr";
  case Moderator:
    return "mod";
  case Monitor:
    return "mon";
  case Marbler:
    return "mrb";
  case MarkupEditor:
    return "mrk";
  case MusicalDirector:
    return "msd";
  case MetalEngraver:
    return "mte";
  case MinuteTaker:
    return "mtk";
  case Musician:
    return "mus";
  case Narrator:
    return "nrt";
  case Opponent:
    return "opn";
  case Originator:
    return "org";
  case Organizer:
    return "orm";
  case OnscreenPresenter:
    return "osp";
  case Other:
    return "oth";
  case Owner:
    return "own";
  case Panelist:
    return "pan";
  case Patron:
    return "pat";
  case PublishingDirector:
    return "pbd";
  case Publisher:
    return "pbl";
  case ProjectDirector:
    return "pdr";
  case Proofreader:
    return "pfr";
  case Photographer:
    return "pht";
  case Platemaker:
    return "plt";
  case PermittingAgency:
    return "pma";
  case ProductionManager:
    return "pmn";
  case PrinterOfPlates:
    return "pop";
  case Papermaker:
    return "ppm";
  case Puppeteer:
    return "ppt";
  case Praeses:
    return "pra";
  case ProcessContact:
    return "prc";
  case ProductionPersonnel:
    return "prd";
  case Presenter:
    return "pre";
  case Performer:
    return "prf";
  case Programmer:
    return "prg";
  case Printmaker:
    return "prm";
  case ProductionCompany:
    return "prn";
  case Producer:
    return "pro";
  case ProductionPlace:
    return "prp";
  case ProductionDesigner:
    return "prs";
  case Printer:
    return "prt";
  case Provider:
    return "prv";
  case PatentApplicant:
    return "pta";
  case PlaintiffAppellee:
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
    relator.setType( Abridger);
  } else if (name == "acp") {
    relator.setType( ArtCopyist);
  } else if (name == "act") {
    relator.setType( Actor);
  } else if (name == "adi") {
    relator.setType( ArtDirector);
  } else if (name == "adp") {
    relator.setType( Adapter);
  } else if (name == "aft") {
    relator.setType( AuthorOfAfterword);
  } else if (name == "anl") {
    relator.setType( Analyst);
  } else if (name == "anm") {
    relator.setType( Animator);
  } else if (name == "ann") {
    relator.setType( Annotator);
  } else if (name == "ant") {
    relator.setType( BibliographicAntecedent);
  } else if (name == "ape") {
    relator.setType( Appellee);
  } else if (name == "apl") {
    relator.setType( Appellant);
  } else if (name == "app") {
    relator.setType( Applicant);
  } else if (name == "aqt") {
    relator.setType( AuthorInQuotations);
  } else if (name == "arc") {
    relator.setType( Architect);
  } else if (name == "ard") {
    relator.setType( ArtisticDirector);
  } else if (name == "arr") {
    relator.setType( Arranger);
  } else if (name == "art") {
    relator.setType( Artist);
  } else if (name == "asg") {
    relator.setType( Assignee);
  } else if (name == "asn") {
    relator.setType( AssociatedName);
  } else if (name == "ato") {
    relator.setType( Autographer);
  } else if (name == "att") {
    relator.setType( AttributedName);
  } else if (name == "auc") {
    relator.setType( Auctioneer);
  } else if (name == "aut") {
    relator.setType( Author);
  } else if (name == "aud") {
    relator.setType( AuthorOfDialog);
  } else if (name == "aui") {
    relator.setType( AuthorOfIntroduction);
  } else if (name == "aus") {
    relator.setType( Screenwriter);
  } else if (name == "bdd") {
    relator.setType( BindingDesigner);
  } else if (name == "bjd") {
    relator.setType( BookjacketDesigner);
  } else if (name == "bkd") {
    relator.setType( BookDesigner);
  } else if (name == "bkp") {
    relator.setType( BookProducer);
  } else if (name == "blw") {
    relator.setType( BlurbWriter);
  } else if (name == "bnd") {
    relator.setType( Binder);
  } else if (name == "bpd") {
    relator.setType( BookplateDesigner);
  } else if (name == "brd") {
    relator.setType( Broadcaster);
  } else if (name == "brl") {
    relator.setType( BrailleEmbosser);
  } else if (name == "bsl") {
    relator.setType( Bookseller);
  } else if (name == "cas") {
    relator.setType( Caster);
  } else if (name == "ccp") {
    relator.setType( Conceptor);
  } else if (name == "chr") {
    relator.setType( Choreographer);
  } else if (name == "clb") {
    relator.setType( Collaborator); // Discontinued
  } else if (name == "cli") {
    relator.setType( Client);
  } else if (name == "cll") {
    relator.setType( Calligrapher);
  } else if (name == "clr") {
    relator.setType( Colorist);
  } else if (name == "clt") {
    relator.setType( Collotyper);
  } else if (name == "cmm") {
    relator.setType( Commentator);
  } else if (name == "cmp") {
    relator.setType( Composer);
  } else if (name == "cmt") {
    relator.setType( Compositor);
  } else if (name == "cnd") {
    relator.setType( Conductor);
  } else if (name == "cng") {
    relator.setType( Cinematographer);
  } else if (name == "cns") {
    relator.setType( Censor);
  } else if (name == "coe") {
    relator.setType( ContestantAppellee);
  } else if (name == "col") {
    relator.setType( Collector);
  } else if (name == "com") {
    relator.setType( Compiler);
  } else if (name == "con") {
    relator.setType( Conservator);
  } else if (name == "cor") {
    relator.setType( CollectionRegistrar);
  } else if (name == "cos") {
    relator.setType( Contestant);
  } else if (name == "cot") {
    relator.setType( ContesteeAppellant);
  } else if (name == "cou") {
    relator.setType( CourtGoverned);
  } else if (name == "cov") {
    relator.setType( CoverDesigner);
  } else if (name == "cpc") {
    relator.setType( CopyrightClaimant);
  } else if (name == "cpe") {
    relator.setType( ComplainantAppellee);
  } else if (name == "cph") {
    relator.setType( CopyrightHolder);
  } else if (name == "cpl") {
    relator.setType( Complainant);
  } else if (name == "cpt") {
    relator.setType( ComplainantAppellant);
  } else if (name == "cre") {
    relator.setType( Creator);
  } else if (name == "crp") {
    relator.setType( Correspondent);
  } else if (name == "crr") {
    relator.setType( Corrector);
  } else if (name == "crt") {
    relator.setType( CourtReporter);
  } else if (name == "csl") {
    relator.setType( Consultant);
  } else if (name == "csp") {
    relator.setType( ConsultantToAProject);
  } else if (name == "cst") {
    relator.setType( CostumeDesigner);
  } else if (name == "ctb") {
    relator.setType( Contributor);
  } else if (name == "cte") {
    relator.setType( ContesteeAppellee);
  } else if (name == "ctg") {
    relator.setType( Cartographer);
  } else if (name == "ctr") {
    relator.setType( Contractor);
  } else if (name == "cts") {
    relator.setType( Contestee);
  } else if (name == "ctt") {
    relator.setType( ContestantAppellant);
  } else if (name == "cur") {
    relator.setType( Curator);
  } else if (name == "cwt") {
    relator.setType( CommentatorForWrittenText);
  } else if (name == "dbp") {
    relator.setType( DistributionPlace);
  } else if (name == "dfd") {
    relator.setType( Defendant);
  } else if (name == "dfe") {
    relator.setType( DefendantAppellee);
  } else if (name == "dft") {
    relator.setType( DefendantAppellant);
  } else if (name == "dgg") {
    relator.setType( DegreeGrantingInstitution);
  } else if (name == "dgs") {
    relator.setType( DegreeSupervisor);
  } else if (name == "dis") {
    relator.setType( Dissertant);
  } else if (name == "dln") {
    relator.setType( Delineator);
  } else if (name == "dnc") {
    relator.setType( Dancer);
  } else if (name == "dnr") {
    relator.setType( Donor);
  } else if (name == "dpc") {
    relator.setType( Depicted);
  } else if (name == "dpt") {
    relator.setType( Depositor);
  } else if (name == "drm") {
    relator.setType( Draftsman);
  } else if (name == "drt") {
    relator.setType( Director);
  } else if (name == "dsr") {
    relator.setType( Designer);
  } else if (name == "dst") {
    relator.setType( Distributor);
  } else if (name == "dtc") {
    relator.setType( DataContributor);
  } else if (name == "dte") {
    relator.setType( Dedicatee);
  } else if (name == "dtm") {
    relator.setType( DataManager);
  } else if (name == "dto") {
    relator.setType( Dedicator);
  } else if (name == "dub") {
    relator.setType( DubiousAuthor);
  } else if (name == "edc") {
    relator.setType( EditorOfCompilation);
  } else if (name == "edm") {
    relator.setType( EditorOfMovingImageWork);
  } else if (name == "edt") {
    relator.setType( Editor);
  } else if (name == "egr") {
    relator.setType( Engraver);
  } else if (name == "elg") {
    relator.setType( Electrician);
  } else if (name == "elt") {
    relator.setType( Electrotyper);
  } else if (name == "eng") {
    relator.setType( Engineer);
  } else if (name == "enj") {
    relator.setType( EnactingJurisdiction);
  } else if (name == "etr") {
    relator.setType( Etcher);
  } else if (name == "evp") {
    relator.setType( EventPlace);
  } else if (name == "exp") {
    relator.setType( Expert);
  } else if (name == "fac") {
    relator.setType( Facsimilist);
  } else if (name == "fds") {
    relator.setType( FilmDistributor);
  } else if (name == "fld") {
    relator.setType( FieldDirector);
  } else if (name == "flm") {
    relator.setType( FilmEditor);
  } else if (name == "fmd") {
    relator.setType( FilmDirector);
  } else if (name == "fmk") {
    relator.setType( Filmmaker);
  } else if (name == "fmo") {
    relator.setType( FormerOwner);
  } else if (name == "fmp") {
    relator.setType( FilmProducer);
  } else if (name == "fnd") {
    relator.setType( Funder);
  } else if (name == "fpy") {
    relator.setType( FirstParty);
  } else if (name == "frg") {
    relator.setType( Forger);
  } else if (name == "gis") {
    relator.setType( GeographicInformationSpecialist);
  } else if (name == "grt") {
    relator.setType( GraphicTechnician); // Discontinued
  } else if (name == "his") {
    relator.setType( HostInstitution);
  } else if (name == "hnr") {
    relator.setType( Honoree);
  } else if (name == "hst") {
    relator.setType( Host);
  } else if (name == "ill") {
    relator.setType( Illustrator);
  } else if (name == "ilu") {
    relator.setType( Illuminator);
  } else if (name == "ins") {
    relator.setType( Inscriber);
  } else if (name == "inv") {
    relator.setType( Inventor);
  } else if (name == "isb") {
    relator.setType( IssuingBody);
  } else if (name == "itr") {
    relator.setType( Instrumentalist);
  } else if (name == "ive") {
    relator.setType( Interviewee);
  } else if (name == "ivr") {
    relator.setType( Interviewer);
  } else if (name == "jud") {
    relator.setType( Judge);
  } else if (name == "jug") {
    relator.setType( JurisdictionGoverned);
  } else if (name == "lbr") {
    relator.setType( Laboratory);
  } else if (name == "lbt") {
    relator.setType( Librettist);
  } else if (name == "ldr") {
    relator.setType( LaboratoryDirector);
  } else if (name == "led") {
    relator.setType( Lead);
  } else if (name == "lee") {
    relator.setType( LibeleeAppellee);
  } else if (name == "lel") {
    relator.setType( Libelee);
  } else if (name == "len") {
    relator.setType( Lender);
  } else if (name == "let") {
    relator.setType( LibeleeAppellant);
  } else if (name == "lgd") {
    relator.setType( LightingDesigner);
  } else if (name == "lie") {
    relator.setType( LibelantAppellee);
  } else if (name == "lil") {
    relator.setType( Libelant);
  } else if (name == "lit") {
    relator.setType( LibelantAppellant);
  } else if (name == "lsa") {
    relator.setType( LandscapeArchitect);
  } else if (name == "lse") {
    relator.setType( Licensee);
  } else if (name == "lso") {
    relator.setType( Licensor);
  } else if (name == "ltg") {
    relator.setType( Lithographer);
  } else if (name == "lyr") {
    relator.setType( Lyricist);
  } else if (name == "mcp") {
    relator.setType( Author);
  } else if (name == "mdc") {
    relator.setType( MetadataContact);
  } else if (name == "med") {
    relator.setType( Medium);
  } else if (name == "mfp") {
    relator.setType( ManufacturePlace);
  } else if (name == "mfr") {
    relator.setType( Manufacturer);
  } else if (name == "mod") {
    relator.setType( Moderator);
  } else if (name == "mon") {
    relator.setType( Monitor);
  } else if (name == "mrb") {
    relator.setType( Marbler);
  } else if (name == "mrk") {
    relator.setType( MarkupEditor);
  } else if (name == "msd") {
    relator.setType( MusicalDirector);
  } else if (name == "mte") {
    relator.setType( MetalEngraver);
  } else if (name == "mtk") {
    relator.setType( MinuteTaker);
  } else if (name == "mus") {
    relator.setType( Musician);
  } else if (name == "nrt") {
    relator.setType( Narrator);
  } else if (name == "opn") {
    relator.setType( Opponent);
  } else if (name == "org") {
    relator.setType( Originator);
  } else if (name == "orm") {
    relator.setType( Organizer);
  } else if (name == "osp") {
    relator.setType( OnscreenPresenter);
  } else if (name == "oth") {
    relator.setType( Other);
  } else if (name == "own") {
    relator.setType( Owner);
  } else if (name == "pan") {
    relator.setType( Panelist);
  } else if (name == "pat") {
    relator.setType( Patron);
  } else if (name == "pbd") {
    relator.setType( PublishingDirector);
  } else if (name == "pbl") {
    relator.setType( Publisher);
  } else if (name == "pdr") {
    relator.setType( ProjectDirector);
  } else if (name == "pfr") {
    relator.setType( Proofreader);
  } else if (name == "pht") {
    relator.setType( Photographer);
  } else if (name == "plt") {
    relator.setType( Platemaker);
  } else if (name == "pma") {
    relator.setType( PermittingAgency);
  } else if (name == "pmn") {
    relator.setType( ProductionManager);
  } else if (name == "pop") {
    relator.setType( PrinterOfPlates);
  } else if (name == "ppm") {
    relator.setType( Papermaker);
  } else if (name == "ppt") {
    relator.setType( Puppeteer);
  } else if (name == "pra") {
    relator.setType( Praeses);
  } else if (name == "prc") {
    relator.setType( ProcessContact);
  } else if (name == "prd") {
    relator.setType( ProductionPersonnel);
  } else if (name == "pre") {
    relator.setType( Presenter);
  } else if (name == "prf") {
    relator.setType( Performer);
  } else if (name == "prg") {
    relator.setType( Programmer);
  } else if (name == "prm") {
    relator.setType( Printmaker);
  } else if (name == "prn") {
    relator.setType( ProductionCompany);
  } else if (name == "pro") {
    relator.setType( Producer);
  } else if (name == "prp") {
    relator.setType( ProductionPlace);
  } else if (name == "prs") {
    relator.setType( ProductionDesigner);
  } else if (name == "prt") {
    relator.setType( Printer);
  } else if (name == "prv") {
    relator.setType( Provider);
  } else if (name == "pta") {
    relator.setType( PatentApplicant);
  } else if (name == "pte") {
    relator.setType( PlaintiffAppellee);
  } else if (name == "ptf") {
    relator.setType( Plaintiff);
  } else if (name == "pth") {
    relator.setType( PatentHolder);
  } else if (name == "ptt") {
    relator.setType( PlaintiffAppellant);
  } else if (name == "pup") {
    relator.setType( PublicationPlace);
  } else if (name == "rbr") {
    relator.setType( Rubricator);
  } else if (name == "rcd") {
    relator.setType( Recordist);
  } else if (name == "rce") {
    relator.setType( RecordingEngineer);
  } else if (name == "rcp") {
    relator.setType( Addressee);
  } else if (name == "rdd") {
    relator.setType( RadioDirector);
  } else if (name == "red") {
    relator.setType( Redaktor);
  } else if (name == "ren") {
    relator.setType( Renderer);
  } else if (name == "res") {
    relator.setType( Researcher);
  } else if (name == "rev") {
    relator.setType( Reviewer);
  } else if (name == "rpc") {
    relator.setType( RadioProducer);
  } else if (name == "rps") {
    relator.setType( Repository);
  } else if (name == "rpt") {
    relator.setType( Reporter);
  } else if (name == "rpy") {
    relator.setType( ResponsibleParty);
  } else if (name == "rse") {
    relator.setType( RespondentAppellee);
  } else if (name == "rsg") {
    relator.setType( Restager);
  } else if (name == "rsp") {
    relator.setType( Respondent);
  } else if (name == "rsr") {
    relator.setType( Restorationist);
  } else if (name == "rst") {
    relator.setType( RespondentAppellant);
  } else if (name == "rth") {
    relator.setType( ResearchTeamHead);
  } else if (name == "rtm") {
    relator.setType( ResearchTeamMember);
  } else if (name == "sad") {
    relator.setType( ScientificAdvisor);
  } else if (name == "sce") {
    relator.setType( Scenarist);
  } else if (name == "scl") {
    relator.setType( Sculptor);
  } else if (name == "scr") {
    relator.setType( Scribe);
  } else if (name == "sds") {
    relator.setType( SoundDesigner);
  } else if (name == "sec") {
    relator.setType( Secretary);
  } else if (name == "sgd") {
    relator.setType( StageDirector);
  } else if (name == "sgn") {
    relator.setType( Signer);
  } else if (name == "sht") {
    relator.setType( SupportingHost);
  } else if (name == "sll") {
    relator.setType( Seller);
  } else if (name == "sng") {
    relator.setType( Singer);
  } else if (name == "spk") {
    relator.setType( Speaker);
  } else if (name == "spn") {
    relator.setType( Sponsor);
  } else if (name == "spy") {
    relator.setType( SecondParty);
  } else if (name == "srv") {
    relator.setType( Surveyor);
  } else if (name == "std") {
    relator.setType( SetDesigner);
  } else if (name == "stg") {
    relator.setType( Setting);
  } else if (name == "stl") {
    relator.setType( Storyteller);
  } else if (name == "stm") {
    relator.setType( StageManager);
  } else if (name == "stn") {
    relator.setType( StandardsBody);
  } else if (name == "str") {
    relator.setType( Stereotyper);
  } else if (name == "tcd") {
    relator.setType( TechnicalDirector);
  } else if (name == "tch") {
    relator.setType( Teacher);
  } else if (name == "ths") {
    relator.setType( ThesisAdvisor);
  } else if (name == "tld") {
    relator.setType( TelevisionDirector);
  } else if (name == "tlp") {
    relator.setType( TelevisionProducer);
  } else if (name == "trc") {
    relator.setType( Transcriber);
  } else if (name == "trl") {
    relator.setType( Translator);
  } else if (name == "tyd") {
    relator.setType( TypeDesigner);
  } else if (name == "tyg") {
    relator.setType( Typographer);
  } else if (name == "uvp") {
    relator.setType( UniversityPlace);
  } else if (name == "vac") {
    relator.setType( VoiceActor);
  } else if (name == "vdg") {
    relator.setType( Videographer);
  } else if (name == "voc") {
    relator.setType( Vocalist); // Discontinued
  } else if (name == "wac") {
    relator.setType( WriterOfAddedCommentary);
  } else if (name == "wal") {
    relator.setType( WriterOfAddedLyrics);
  } else if (name == "wam") {
    relator.setType( WriterOfAccompanyingMaterial);
  } else if (name == "wat") {
    relator.setType( WriterOfAddedText);
  } else if (name == "wdc") {
    relator.setType( Woodcutter);
  } else if (name == "wde") {
    relator.setType( WoodEngraver);
  } else if (name == "win") {
    relator.setType( WriterOfIntroduction);
  } else if (name == "wit") {
    relator.setType( Witness);
  } else if (name == "wpr") {
    relator.setType( WriterOfPreface);
  } else if (name == "wst") {
    relator.setType( WriterOfSupplementaryTextualContent);
  } else {
    relator.setType( no_type);
  }
  return relator;
}


