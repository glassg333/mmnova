#!/usr/bin/env python3
"""Static integrity checks for the 1.9.17 source package and retained native DSP.

Usage:
    python3 verify_dsp_mode_patch.py Source

The argument is the project's Source directory.  This check intentionally does
not replace a compiler/test run; it guards the release inventory against a
withdrawn selector or unrelated imported DSP branch being reintroduced accidentally.
The one reviewed, explicitly isolated FM+ NEW import is guarded here too.
"""

from __future__ import annotations

from pathlib import Path
import hashlib
import re
import sys


def main() -> int:
    source = Path(sys.argv[1] if len(sys.argv) > 1 else "Source").resolve()
    project = source.parent
    failures: list[str] = []

    def check(condition: bool, message: str) -> None:
        print(("OK   " if condition else "FAIL ") + message)
        if not condition:
            failures.append(message)

    def text(relative: str) -> str:
        path = source / relative
        check(path.is_file(), f"present: Source/{relative}")
        return path.read_text(encoding="utf-8", errors="replace") if path.is_file() else ""

    modes = text("models/DspModes.hpp")
    data = text("NovaData.h")
    dsp = text("NovaDSP.h")
    processor = text("PluginProcessor.cpp")
    editor = text("PluginEditor.cpp")
    dist = text("dsp/TrackDIST.inl")
    filt = text("dsp/TrackFILT.inl")
    srr = text("dsp/TrackSRR.inl")
    delay = text("dsp/TrackDelay.inl")
    fm_fix_tables = text("dsp/fm_fix/FmFixTables.hpp")
    mnm_fix_fm = text("dsp/fm_fix/MnmFmFix.hpp")
    mnm_fm = text("dsp/mnm/MnmFm.hpp")
    machine_definitions = text("models/machine_definitions.hpp")
    fm_new_wrapper = text("dsp/fm_new/FmExactNew.hpp")
    fm_new_dsp = text("dsp/fm_new/FmExactDsp.hpp")
    fm_new_stat = text("dsp/fm_new/FmExactStat.hpp")
    fm_new_par = text("dsp/fm_new/FmExactPar.hpp")
    fm_new_dyn = text("dsp/fm_new/FmExactDyn.hpp")
    fm_new_sine = text("dsp/fm_new/FmExactSineTable.h")
    fm_new_readme = text("dsp/fm_new/README.md")
    fm_new_fix_wrapper = text("dsp/fm_new_fix/FmExactNewFix.hpp")
    fm_new_fix_dsp = text("dsp/fm_new_fix/FmExactFixDsp.hpp")
    fm_new_fix_stat = text("dsp/fm_new_fix/FmExactFixStat.hpp")
    fm_new_fix_par = text("dsp/fm_new_fix/FmExactFixPar.hpp")
    fm_new_fix_dyn = text("dsp/fm_new_fix/FmExactFixDyn.hpp")
    cmake = (project / "CMakeLists.txt").read_text(encoding="utf-8", errors="replace")
    jucers = list(project.glob("*.jucer"))
    jucer = jucers[0].read_text(encoding="utf-8", errors="replace") if len(jucers) == 1 else ""
    generated_header = (project / "JuceLibraryCode/JuceHeader.h").read_text(encoding="utf-8", errors="replace")
    generated_defines = (project / "JuceLibraryCode/JucePluginDefines.h").read_text(encoding="utf-8", errors="replace")

    check('dspModeCount = 5' in modes and 'dspModeNew = 2' in modes
          and 'dspModeMnmFix = 3' in modes and 'dspModeNewFix = 4' in modes,
          "DSP registry preserves mnm/old/new IDs and appends MNM FIX=3 / NEW FIX=4")
    check('kDspModeSchemaVersion = 30' in modes,
          "DSP schema 30 retains prior IDs and adds FM+ m8/m9/m10-only FIX candidates")
    for section in ("DspSynt", "DspFilter", "DspDist"):
        check(re.search(rf'case {section}:\s*return "mnm\|old";', modes) is not None,
              f"{section} retains only mnm|old")
    check(re.search(r'case DspDelay:\s*return "mnm\|old\|new";', modes) is not None,
          "DspDelay alone exposes appended mnm|old|new")
    for forbidden in ("dspModeFma", "dspModeSyntNew"):
        check(forbidden not in modes, f"registry has no {forbidden}")
    check('dspSyntSupportsNewForMachine' in modes
          and 'dspSyntSupportsFixForMachine' in modes
          and 'machineId == 8 || machineId == 9 || machineId == 10' in modes
          and '"mnm|old|new|mnm fix|new fix"' in modes
          and 'dspSyntModeChoicesForMachine' in modes
          and 'dspSyntModeAllowedForMachine' in modes,
          "SYNT NEW/FIX has an explicit m8/m9/m10-only allow-list")

    check('"old|mnm|vital"' in data, "AMP retains old|mnm|vital")
    check('defaultMode=(section==monomachine::DspFilter||section==monomachine::DspDelay)' in data
          and '"p2_mode_filt"' in data and '"p2_mode_dly"' in data
          and 'section == DspFilter || section == DspDelay' in modes,
          "new P1/P2 FILT and DLY instances default to OLD without changing mode IDs")
    check('"MNM|OLD|MNM FIX|FOLD|ZERO|CLAMP|MNM+OLD|MNM V2|OLD V2"' in data
          and '0,8,0,1' in data,
          "MODE S retains its three explicit A/B candidates without changing IDs 0..5")
    check('syntModeCount=monomachine::dspSyntModeCountForMachine(m.id)' in data
          and 'dspSyntModeChoicesForMachine(m.id)' in data
          and 'MNM FIX=3 and NEW FIX=4' in data,
          "APVTS appends NEW/FIX only to the three FM machine-local SYNT parameters")
    check('selectedMachineId' in processor and 'dspSyntModeAllowedForMachine(selectedMachineId,idx)' in processor
          and 'schema>=29 && value==monomachine::dspModeNew' in processor
          and 'schema>=30 && (value==monomachine::dspModeMnmFix' in processor
          and 'dspSyntSupportsFixForMachine(machineId)' in processor
          and 'migrateSyntMode' in processor,
          "snapshot and schema migration accept NEW/FIX only for their FM+ state eras")
    check('dspSyntModeCountForMachine(selectedMachineId)' in editor
          and 'dspSyntModeAllowedForMachine(selectedMachineId,mode)' in editor
          and 'dspSyntModeChoicesForMachine(machineId)' in editor,
          "SYNT popup, face MODE SYNT and callback share the FM-only NEW/FIX list")
    check('kHybridFilterChoiceCount = 58' in data
          and 'R 303 HP' in data and 'R MS20 HP' in data and 'R MOOG HP24' in data
          and 'R ANALOG HP24' in data and 'R LINEAR HP24' in data and 'R RBJ HP' in data
          and 'R TPT HP' in data and 'R HUV LP4' in data and 'R HYPER HP4' in data
          and 'R DVAL LP4' in data and 'R HUV HP4' in data and 'R KRAJ HP4' in data
          and 'R MICRO HP4' in data and 'R MUSIC HP4' in data
          and 'R OBERHEIM HP4' in data and 'R DVAL HP4' in data,
          "MODE L/H retain all legacy choices and append six derived R Import 2 HP choices")
    check('hybridDistMnmFix' in dist and 'processMnmPreDriven' in dist,
          "MNM FIX and direct pre-driven imported DIST paths are active")
    check('p2_fx_mode' not in data, "P2 experimental selector is absent from parameter layout")
    check('p2FxModeRaw' not in processor, "P2 experimental selector has no runtime field")
    check('state.removeChild(p2FxMode,nullptr)' in processor,
          "legacy p2_fx_mode is discarded while loading state")
    check('nativeMode("p2_mode_dist")' in processor and '"p2_amp_mode"' in processor,
          "P1/P2 withdrawn values are explicitly migrated")
    check('schema<20' in processor and 'filterDefaults[]{0,0,0,0,0,0,0,127,127,0,0,0}' in processor,
          "schema-20 filter-extra migration inserts neutral values")
    check('schema<21' in processor and '"arp_step_start"' in processor
          and '"arp_step_end"' in processor and 'arp_step_page_limit' in processor,
          "schema-21 migrates legacy ARP pages into the live 1..64 window")
    check('schema<22' in processor and '"plock_sync"' in processor
          and '"plock_step_start"' in processor and '"plock_step_end"' in processor,
          "schema-22 preserves legacy P-LOCK sync while adding its independent window")
    check('schema<23' in processor and 'migrateDelayMode("mode_dly")' in processor
          and 'migrateDelayMode("p2_mode_dly")' in processor
          and '"dly_dbas_q"' in processor and '"p2_dly_dwid_q"' in processor,
          "schema-23 collapses legacy DLY value 2 to MNM and adds neutral P1/P2 feedback-Q")
    check('migrateHybridFilterMode' in processor and 'schema<24' in processor
          and 'schema<25' in processor and 'value>=0&&value<=20' in processor
          and 'nova::kHybridFilterChoiceCount-1' in processor
          and '"hybrid_p1_mode_l"' in processor and '"hybrid_p2_mode_h"' in processor,
          "schema-25 preserves 0..20, appends R Import 2, and rejects malformed old values")
    check('"dly_dbas_q"' in data and '"dly_dwid_q"' in data
          and '"p2_dly_dbas_q"' in data and '"p2_dly_dwid_q"' in data,
          "DBAS/DWID optional Q parameters are registered for P1 and P2")
    check('"filt_"' in data and '"p2_filt_"' in data and '"ENV FIL"' in data,
          "P1/P2 opt-in FILT extra parameters are registered")
    check('"track_hpf"' in data and '"track_lpf"' in data
          and 'HPF KEYTRACK' in data and 'LPF KEYTRACK' in data,
          "P1/P2 independent binary HPF/LPF key-tracking switches are registered")
    check('schema<26' in processor and 'filt_track_hpf' in processor
          and 'p2_filt_track_lpf' in processor and 'setFilterKeyTracking' in processor,
          "schema-26 migrates absent key-tracking controls to ON and feeds both chains")
    check('schema<27' in processor and 'hybrid_p1_mode_s' in processor
          and 'hybrid_p2_mode_s' in processor and 'value>=0&&value<=5' in processor,
          "schema-27 preserves existing MODE S 0..5 values and does not reinterpret old projects as candidates")
    check('schema<28' in processor and 'value>=0&&value<=51' in processor
          and 'nova::kHybridFilterChoiceCount-1' in processor,
          "schema-28 protects pre-extension projects from interpreting new IDs 52..57")
    check('kStatParRatioWord' in fm_fix_tables and 'ONE canonical numerical FIX table' in fm_fix_tables
          and 'return static_cast<float>(kStatParRatioWord' in fm_fix_tables
          and '"5/32"' in fm_fix_tables and 'dynRatio1ForRaw' in fm_fix_tables
          and 'dynRatio2ForRaw' in fm_fix_tables and 'tuneSemitonesForRaw' in fm_fix_tables
          and 'kStatFactoryRaw' in fm_fix_tables and 'factoryDefaultsForMachine' in fm_fix_tables,
          "FM FIX uses one canonical STAT/PAR word table plus explicit FIX-only DYN/TUNE/default data")
    check('{"1FRQ", "Carrier 1 Frequency", 0, 127, 16}' in machine_definitions
          and '{"2FRQ", "Modulator 2 Frequency", 0, 127, 32}' in machine_definitions
          and '{"3FRQ", "Operator 3 Frequency", 0, 127, 48}' in machine_definitions
          and 'FIX factory words are opt-in only' in machine_definitions,
          "FM+ global parameter defaults remain the retained baseline; recovered FIX defaults are not global")
    check('class MnmFixCore' in mnm_fix_fm and 'MnmFixKind' in mnm_fix_fm
          and 'statParRatioForRaw' in mnm_fix_fm and 'dynRatio1ForRaw' in mnm_fix_fm
          and 'tuneSemitonesForRaw' in mnm_fix_fm
          and 'setFixControlLaws' not in mnm_fm,
          "MNM FIX owns a separate core; retained MnmFm.hpp has no FIX flag or FIX table dependency")
    check('namespace monomachine {\nnamespace fm_new' in fm_new_wrapper
          and 'namespace fmnew' in fm_new_dsp and 'namespace fmnew' in fm_new_stat
          and 'namespace fmnew' in fm_new_par and 'namespace fmnew' in fm_new_dyn
          and 'setPitchWordOverride' in fm_new_wrapper and 'fifoLen' in fm_new_wrapper
          and 'setFixControlLaws' not in fm_new_wrapper and 'setRatioTable' not in fm_new_dsp
          and 'class FmExactFixCore' in fm_new_fix_wrapper
          and 'namespace fmnewfix' in fm_new_fix_dsp and 'namespace fmnewfix' in fm_new_fix_stat
          and 'namespace fmnewfix' in fm_new_fix_par and 'namespace fmnewfix' in fm_new_fix_dyn
          and 'configureMeasuredTable' in fm_new_fix_wrapper and 'setRatioTable' in fm_new_fix_dsp
          and '#include "dsp/mnm/' not in fm_new_wrapper and '#include "MnmKernel.hpp"' not in fm_new_wrapper,
          "retained NEW and NEW FIX use separate namespaced cores; retained NEW has no FIX state/API")
    check('* 2ll' in fm_new_dsp and '52,800-word STAT vectors' in fm_new_dsp
          and '#include "FmExactSineTable.h"' in fm_new_dsp
          and 'MnmFmSineTable.h' not in fm_new_dsp,
          "FM NEW DSP uses the reviewed portable arithmetic repair and isolated LUT")
    check('52,800/52,800' in fm_new_readme and 'PAR/DYN vector corpus' in fm_new_readme
          and 'pitch-word' in fm_new_readme,
          "FM NEW provenance records the STAT proof and remaining PAR/DYN/pitch boundary")
    check('#include "dsp/fm_new/FmExactNew.hpp"' in dsp
          and '#include "dsp/fm_new_fix/FmExactNewFix.hpp"' in dsp
          and '#include "dsp/fm_fix/MnmFmFix.hpp"' in dsp
          and 'const bool newExact=syntMode==monomachine::dspModeNew&&isFm;' in dsp
          and 'const bool newFix=syntMode==monomachine::dspModeNewFix&&isFm;' in dsp
          and 'const bool mnmFix=syntMode==monomachine::dspModeMnmFix&&isFm;' in dsp
          and 'fmNewFix.setParameters(kind,p)' in dsp and 'mnmFmFix.setParameters(kind,values)' in dsp
          and 'mnmFm.setParameters(kind,values)' in dsp
          and 'fm_fix::tuneSemitonesForRaw' in dsp
          and 'monomachine::fm_new::FmExactFixCore fmNewFix' in dsp
          and 'monomachine::fm_fix::MnmFixCore mnmFmFix' in dsp,
          "NovaDSP gives NEW FIX and MNM FIX independent core/state objects; retained NEW/MNM routes stay separate")
    check('dspSyntModeUsesMeasuredFix' in modes and 'usesMeasuredFmFixReadout' in editor
          and 'const bool measuredFix=usesMeasuredFmFixReadout();' in editor
          and 'measuredFix&&machine==10&&knob==0' in editor
          and 'measuredFix&&(machine==8||machine==9)' in editor
          and 'measuredFix&&(machine==8||machine==9||machine==10)&&label=="TUNE"' in editor
          and '!measuredFix&&machine==10&&knob==0' in editor
          and '!measuredFix&&machine==10&&knob==4' in editor
          and '!measuredFix&&(machine==8||machine==9)' in editor
          and 'getFmListedRatio(static_cast<uint8_t>(raw/4))' in editor
          and editor.find('if(measuredFix&&machine==10&&knob==0)') < editor.find('if(!measuredFix&&machine==10&&knob==0)')
          and editor.find('if(measuredFix&&(machine==8||machine==9)') < editor.find('if(!measuredFix&&(machine==8||machine==9)')
          and 'applyFmFixFactoryDefaults' in editor
          and 'LOAD FIX FACTORY RAW VALUES (explicit)' in editor,
          "FM faceplate keeps original m8/m9/m10 lists for mnm/old/new; measured labels/defaults are FIX-only")
    check('FmFixModeTests' in cmake and 'tests/FmFixModeTests.cpp' in cmake,
          "CMake registers the JUCE-free FM FIX regression target")
    legacy_fm_hashes = {
        'dsp/mnm/MnmFm.hpp': '2addde732ce2d1550018e1450ebd370cd5a3c0290dfb9604e3c3c80cb859a23d',
        'dsp/fm_new/FmExactNew.hpp': '9271249e7433c85961d32e28e4b4ee6b457dc1e8d50d0268d8246dcc8adafa3b',
        'dsp/fm_new/FmExactDsp.hpp': '64e2adbbd2ac0f3d6d859514ea98599de24757f92ca5531e150fa051d2cab0d2',
        'dsp/monomachine_fm_stat.hpp': 'c36af9f686279dbbd78c34704aae5f4b0ea79242b1d5a6c29b92fe32a587c801',
        'dsp/monomachine_fm_par.hpp': '45aa99ae41f23992a8aa43e3d06ef0598ccd52c5c9caa36471e5e76644293171',
        'dsp/monomachine_fm_dynamic.hpp': '695ca711b7dc6ca9db2cad389149ffb32df19577a081c728b327b69db18f6033',
    }
    for relative, expected_hash in legacy_fm_hashes.items():
        path = source / relative
        actual_hash = hashlib.sha256(path.read_bytes()).hexdigest() if path.is_file() else ''
        check(actual_hash == expected_hash, f"legacy FM source is byte-stable: Source/{relative}")
    check('filterTrackHpf' in dsp and 'filterTrackLpf' in dsp
          and 'setExternalFilterModifiers(lower,upper,0.0f,0.0f,filterTrackHpf,filterTrackLpf,filterMidiNote)' in dsp
          and 'filterTrackHpf,filterTrackLpf,filterMidiNote' in filt,
          "TrackChain passes independent switches and current played note into both filter routes")
    check('MOD ENV1' in data and '0,35,0,1' in data and '0,36,0,1' in data,
          "four MOD ENV source IDs and AUX encoding are registered")
    check('lfoDirectTarget' in data and 'lfoPageForDirectTarget' in data
          and 'kLfoPageChoiceCount = 21' in data,
          "canonical six-LFO P1/P2 PAGE/DEST mapping is registered")
    matrix = text("models/modulation_matrix.hpp")
    check('ModEnv1 = 32' in matrix and 'ModEnv4' in matrix and 'setModEnvOutputs' in matrix,
          "matrix exposes appended MOD ENV1..4 sources")
    check('kTargetCount = 248' in matrix and 'ARP START' in matrix and 'P-LOCK END' in matrix
          and 'arpWindowDests' in matrix and 'plockWindowDests' in matrix
          and '0,248,0,1,targets' in data and 'jlimit(0,248' in processor,
          "matrix exposes ARP/P-LOCK START/END destinations 244..247 end-to-end")
    check('applyArpAndPlockWindowModulation' in processor and 'plockStepForArpEvent' in processor
          and 'plockStep(plockStepForArpEvent(arp.stepEcho))' in processor,
          "DSP applies independent P-LOCK cursor/window timing on ARP note events")
    check('FilterExtraPanel' in editor and 'FilterEnvelopePage' in editor and 'ModEnvelopePage' in editor,
          "FILT RMB panel and AMP/FIL/MOD ENV overlay pages are present")
    check('openFilterExtras' in editor and 'pageIndex==1&&(knob==0||knob==1)' in editor
          and 'filtCatButton.rightClick={};' in editor
          and 'HPF KEYTRACK' in editor and 'LPF KEYTRACK' in editor,
          "RMB BASE/WDTH owns FILT extras; FILT header no longer opens it; checkboxes are visible")
    check('FilterEnvelopeMiniPage' in editor and 'ModEnvelopeMiniPage' in editor
          and 'filterEnvButton' in editor and 'modEnvButton' in editor
          and 'toggleFilterEnvMini' in editor and 'toggleModEnvMini' in editor,
          "main AMP-adjacent FIL ENV/MOD ENV compact views retain RMB large/compact navigation")
    check('setFilterExtras(filterExtrasP1Raw,chain);setFilterExtras(filterExtrasP2Raw,chain2);' in processor,
          "P1/P2 FILT extras feed independent chains")
    hybrid = text("dsp/hybrid_private/HybridDSP.hpp")
    reference_filters = text("dsp/hybrid_private/RClassicFilters.hpp")
    import2_filters = text("dsp/hybrid_private/RImport2Filters.hpp")
    check('processMnmPreDriven' in hybrid and 'processMnmControlled' not in hybrid
          and 'postLevel' not in hybrid and 'return retainedMnmOutput + amount' not in hybrid,
          "imported MODE S retains lower MNM half but has no synthetic blend/post-level")
    check('hybridR303Lp = 9' in hybrid and 'hybridRMoogHp12 = 20' in hybrid
          and 'hybridRAnalogLp12 = 21' in hybrid and 'hybridRDvalLp4 = 51' in hybrid
          and 'hybridRHuovilainenHp4 = 52' in hybrid and 'hybridRDvalHp4 = 57' in hybrid
          and 'kHybridFilterAlgorithmCount = 58' in hybrid
          and 'hybridR303Hp, hybridRMs20Hp, hybridRMoogHp24' in hybrid
          and 'hybridRAnalogHp24, hybridRAnalogHp12' in hybrid
          and 'hybridRHyperionLp4, hybridRHyperionLp2' in hybrid
          and 'hybridIsDryDerivedComplement' in hybrid and 'hybridIsHyperionFamily' in hybrid,
          "canonical R IDs retain the six dry-derived diagnostics while exposing helpers to hide them and defer Hyperion")
    check('ReferenceClassicStereo' in hybrid and 'referenceModeForHybrid' in hybrid
          and 'ReferenceImport2Stereo' in hybrid and 'import2ModeForHybrid' in hybrid
          and 'RClassicFilters.hpp' in hybrid and 'RImport2Filters.hpp' in hybrid
          and 'physicalControlRampSamples' in hybrid
          and 'kPhysicalControlRampSeconds = 0.010' in hybrid
          and 'rampRemaining = controlRampSamples' in hybrid,
          "R classic and R Import 2 bridges are stereo and use a 10 ms sample-rate-aware control ramp")
    check('3791a32187b5dcffb0efb6832156a688ab93ece7' in reference_filters
          and 'class TB303Core' in reference_filters and 'class MS20Core' in reference_filters
          and 'class MoogCore' in reference_filters and 'std::tanh' in reference_filters,
          "self-contained R classic adaptation records supplied-source provenance and all three cores")
    check('R Import 2 -- self-contained independent filter-family equivalents.' in import2_filters
          and 'class AnalogCore' in import2_filters and 'class LinearCore' in import2_filters
          and 'class RbjCore' in import2_filters and 'class TptCore' in import2_filters
          and 'class HuovilainenCore' in import2_filters and 'class HyperionCore' in import2_filters
          and 'class Import2Core' in import2_filters and 'new ' not in import2_filters,
          "R Import 2 uses documented self-contained allocation-free equivalents")
    check('normalized TPT cascade' in import2_filters and 'stageGain_ = g / (1.0 + g)' in import2_filters
          and 'g_ = 4.0 * kPi * thermalVoltage_ * frequency /' in import2_filters
          and 'frequency * (1.0 - x)' not in import2_filters,
          "HUV has normalized pass-band gain and DVAL has no open-cutoff sign reversal")
    check('FilterRenderRoute' in hybrid and 'IndependentPhysical' in hybrid
          and 'selectFilterRenderRoute' in hybrid and 'dispatchFilterRenderRoute' in hybrid
          and 'hybridFilterPathRequired' not in hybrid,
          "FILT route separates OLD/MNM native renderers from independent physical families")
    real_filter = text("dsp/mnm/MnmRealFilter.hpp")
    check('sampleRateConfigured_' in real_filter
          and 'std::abs(nextHost - host_) < 1.0e-9' in real_filter
          and 'sampleRateConfigured_ = true;' in real_filter
          and 'hybridReferenceLower_.prepare(host_)' in real_filter
          and 'hybridReferenceUpper_.prepare(host_)' in real_filter
          and 'hybridReferenceImport2Lower_.prepare(host_)' in real_filter
          and 'hybridReferenceImport2Upper_.prepare(host_)' in real_filter,
          "unchanged sample rate is a no-op for Korg/Odin/R stateful filter adapters")
    check('hybridReferenceLower_.reset()' in real_filter
          and 'hybridReferenceUpper_.reset()' in real_filter
          and 'hybridUsesOdinLadder' in real_filter
          and 'hybridReferenceLower_.configure' in real_filter
          and 'hybridReferenceUpper_.configure' in real_filter
          and 'hybridReferenceImport2Lower_.configure' in real_filter
          and 'hybridReferenceImport2Upper_.configure' in real_filter
          and 'hybridUsesReferenceImport2' in real_filter,
          "R-family adapters reset only on an actual mode change and route both physical sides")
    set_modes_match = re.search(
        r'void setModes\(int filterModeIn,int distModeIn,int delayModeIn,int routingModeIn\)\s*\{(.*?)\n    \}',
        dsp, re.S)
    check(set_modes_match is not None and 'mnmFilter.setSampleRate' not in set_modes_match.group(1),
          "per-block DSP-mode snapshots do not re-prepare imported filter state")

    for forbidden in ("MnmNewAdapters", "MnmAmpDistNew", "newdsp::", "FmCoreExact",
                      "EnvExactCore", "mnmFilterExact", "newSrr",
                      "softFmSat", "softFmDist", "fmaActive"):
        check(forbidden not in dsp, f"NovaDSP has no active {forbidden}")
    check('bbox.setFirmwareStart(false);bbox.setFirmwarePitch(false);' in dsp,
          "BBOX uses retained native laws")
    check('dspModeNew' not in dist and 'softFmDist' not in dist,
          "DIST has no FMA/source-derived branch")
    check('dspModeNew' not in filt and 'mnmFilterExact' not in filt,
          "FILT has no source-derived branch")
    check('dispatchFilterRenderRoute(' in filt and 'independentPhysicalFilter' in filt
          and 'renderPhysicalCore(mnmFilter)' in filt
          and 'renderPhysicalCore(independentPhysicalFilter)' in filt
          and 'hybridFilterPathRequired' not in filt,
          "FILT dispatches a selected family only to its independent state, never OLD/MNM serially")
    check('IndependentPhysicalFilterCore' in real_filter
          and 'processIndependentPhysical' in real_filter
          and 'core_.processIndependentPhysical(channel, input)' in real_filter
          and 'physicalSideRendererFor(hybridLowerMode_)' in real_filter
          and 'physicalSideRendererFor(hybridUpperMode_)' in real_filter
          and 'independentPhysicalFilter.setSampleRate(sr)' in dsp
          and 'independentPhysicalFilter.reset()' in dsp,
          "independent physical renderer has distinct lifecycle/state, side replacement, and no generic MNM output clip")
    check('dspModeNew' not in srr and 'newSrr' not in srr,
          "SRR has no source-derived branch")
    check('dspModeNew' in delay and 'newDelay.process' in delay
          and 'delayFilter.processStereo' in delay
          and 'delayBaseQ' in delay and 'delayWidthQ' in delay,
          "DLY dispatches the opt-in NEW candidate and routes DBAS/DWID/Q to all delay paths")
    feedback_filter = text("dsp/mnm/MnmDelayFeedbackFilter.hpp")
    new_delay = text("dsp/mnm/MnmTrackDelayNew.hpp")
    check('Korg35Stereo' in feedback_filter and 'active()' in feedback_filter
          and 'processStereo(float& left, float& right)' in feedback_filter,
          "shared Korg feedback bridge has neutral bypass and independent stereo state")
    check('kControlFrame = 16' in new_delay and 'DelayFeedbackFilter' in new_delay,
          "NEW delay is an independent 16-frame Track Delay candidate")
    check('MnmTrackDelayNew.hpp' in dsp and 'setDelayFeedbackQ' in dsp
          and 'newDelay' in dsp and 'setFeedbackFilterParameters' in dsp,
          "NovaDSP owns NEW delay and delivers independent feedback-Q snapshots")

    check('syntModeCombo.addItem("fma"' not in editor,
          "FMA is absent from the user-facing selector")
    check('DelayFeedbackFilterPanel' in editor and 'showDelayFeedbackQ' in editor
          and 'openDelayFeedbackQ' in editor and 'DBAS Q' in editor and 'DWID Q' in editor,
          "right-click DBAS/DWID exposes the optional matching feedback-Q panel")
    check('showHybridModeMenu' in editor and 'HybridResponse::HighPass' in editor
          and 'HybridResponse::LowPass' in editor and 'addResponse(edge,firstResponse)' in editor
          and 'juce::PopupMenu edge,band,notch,dry;' in editor and 'const auto addDryDerived' in editor
          and 'std::function<bool(int)> visibleItem' in editor and 'steppedIndex' in editor
          and 'if(nova::hybrid_private::hybridIsDryDerivedComplement(algorithm))return lower;' in editor
          and 'hybridIsHyperionFamily(algorithm)==hyperion' in editor
          and 'for(const bool hyperion: {false,true})' in editor
          and 'root.addSubMenu(lower?"LOW CUT / HP":"HIGH CUT / LP",edge)' in editor
          and 'root.addSubMenu("BAND PASS",band)' in editor
          and 'if(lower&&dry.getNumItems()>0)root.addSubMenu("DRY",dry);' in editor
          and 'oppositeResponse' not in editor
          and editor.find('root.addSubMenu("BAND PASS",band);')
              > editor.find('root.addSubMenu(lower?"LOW CUT / HP":"HIGH CUT / LP",edge)')
          and editor.find('root.addSubMenu("DRY",dry);')
              > editor.find('root.addSubMenu("BAND PASS",band);')
          and 'hybridModeLCombo.popupHandler' in editor and 'hybridModeHCombo.popupHandler' in editor,
          "MODE L places direct-input diagnostics in a final DRY folder; MODE H hides them, Hyperion is deferred, and BP is unified")
    check('box.getScreenBounds().getBottom()' not in editor
          and 'JUCE\'s target-component placement chooses above/below' in editor,
          "generic ComboBox popup no longer forces a long menu below the control")
    check('P2 uses the native FX-slot path.' in editor,
          "P2 UI documents the retained native FX-slot path")
    check('lane.setBounds(21, 97, 1129, 197)' in editor
          and 'plck.setBounds(21, 341, 1129, 96)' in editor
          and 'aimBtn.setBounds(399, 312, 40, 26)' in editor
          and 'slotBtn.setBounds(439, 312, 159, 26)' in editor
          and 'polBtn.setBounds(598, 312, 74, 26)' in editor,
          "ARP/P-LOCK default geometry keeps a compact P-LOCK header above its lane")
    check('plockSync' in editor and 'plockStartBox' in editor and 'plockEndBox' in editor
          and 'resetNormalWindow' in editor and 'showBlackValueEntry' in editor,
          "ARP controls implement sync, independent bounds, paired reset and black RMB entry")
    check('shiftNormalSteps' in editor and 'shiftPlockSteps' in editor
          and 'plockActiveStart(),plockActiveEnd()' in editor
          and 'normalShiftLeft.setBounds(68,295,18,22)' in editor
          and 'plockShiftLeft.setBounds(68,314,18,22)' in editor
          and 'normalShiftLeft.getBounds()' in editor and 'plockStartBox.getBounds()' in editor,
          "ARP page has compact independent shift rows with labels that follow saved layout positions")
    check('effxDlyCombo.setBounds(759,335,99,28)' in editor,
          "MODE DLY is positioned above DTIM rather than SRR")
    check('outlineLfoDestination' in editor and 'flashDirectLfoOutline' in editor
          and 'clearDirectLfoOutline' in editor and 'setDirectLfoOutline' not in editor
          and 'outlineLfoDestination(target,0)' in editor
          and 'revealLfoDestination' not in editor and 'p2Button.flashFor(180)' in editor,
          "direct LFO feedback fades once, clears on a patchcord, and P1/P2 flash/navigation is cord-drag-only")
    check('compactDisplayedValue' in editor and 'drawLfoSideBadge' in editor
          and 'full.substring(3)' in editor,
          "P1/P2 names stay canonical in lists while PAGE/DEST faceplates use a compact badge")
    check('overLfoNavigationRegion' in editor and 'updateLfoDragPageNavigation' in editor
          and 'lfoDragPageCandidate' in editor and '<170u' in editor,
          "patchcord LFO-page navigation requires a small inner-region dwell")
    check('alignmentTrack' in editor and 'normalStepTrack' in editor
          and 'x0=190' not in editor,
          "P-LOCK steps derive their width and columns from the normal ARP step track")

    for relative in ("dsp/mnm_new", "dsp/mnm/new", "dsp/mnm/MnmAmpDistNew.hpp",
                     "models/exact_firmware_fm_dispatch.hpp"):
        check(not (source / relative).exists(), f"removed: Source/{relative}")
    for relative in ("tests/DistNewModeSchemaTests.cpp", "tests/DistNewStateTests.cpp",
                     "tests/ImportedNewModesTests.cpp", "tests/MnmAmpDistNewTests.cpp",
                     "tests/TrackDistNewIntegrationTests.cpp", "tests/MnmHeaderOdrLinkTests.cpp"):
        check(not (project / relative).exists(), f"removed: {relative}")
    for forbidden in ("DistNew", "ImportedNew", "MnmAmpDistNew", "MnmHeaderOdr"):
        check(forbidden not in cmake, f"CMake has no {forbidden} target")
    fm_new_tests = (project / "tests/FmNewModeTests.cpp").read_text(encoding="utf-8", errors="replace")
    check('FM NEW STAT raw core retains supplied-vector opening words' in fm_new_tests
          and '5/17/18 host callback splits' in fm_new_tests
          and 'legacy MNM STAT fingerprint remains unchanged' in fm_new_tests
          and 'FmExactKind::Par' in fm_new_tests and 'FmExactKind::Dyn' in fm_new_tests,
          "FM NEW regression covers STAT anchor, callback FIFO, PAR/DYN smoke, and MNM fingerprints")
    fm_fix_tests = (project / "tests/FmFixModeTests.cpp").read_text(encoding="utf-8", errors="replace")
    check('retained global defaults stay baseline; recovered factory profiles are explicit FIX-only data' in fm_fix_tests
          and 'measured table/readout profile is confined to appended FIX mode IDs' in fm_fix_tests
          and 'STAT/PAR literal labels include observed non-monotonic 5/32 slot' in fm_fix_tests
          and 'STAT/PAR factory defaults and MNM float value derive from one exact word table' in fm_fix_tests
          and 'MNM FIX STAT uses a separate opt-in core with measured frequency law' in fm_fix_tests
          and 'NEW FIX STAT uses a separate measured-table core while retained NEW remains separate' in fm_fix_tests
          and 'DYN 1FRQ follows measured direct K/64 control law' in fm_fix_tests
          and 'FIX TUNE endpoints are -64=-2 semitones' in fm_fix_tests
          and 'NEW/NEW FIX state objects are explicitly separate and deterministic' in fm_fix_tests,
          "FM FIX regression covers baseline isolation, explicit defaults, one source table, DYN/TUNE laws, and route isolation")
    hybrid_tests = (project / "tests/HybridDspTests.cpp").read_text(encoding="utf-8", errors="replace")
    real_filter_tests = (project / "tests/MnmRealFilterTests.cpp").read_text(encoding="utf-8", errors="replace")
    rollback_tests = (project / "tests/RollbackStateTests.cpp").read_text(encoding="utf-8", errors="replace")
    import2_tests = (project / "tests/Import2FiltersTests.cpp").read_text(encoding="utf-8", errors="replace")
    filter_route_tests = (project / "tests/FilterRouteTests.cpp").read_text(encoding="utf-8", errors="replace")
    track_filter_route_tests = (project / "tests/TrackFilterRouteIntegrationTests.cpp").read_text(encoding="utf-8", errors="replace")
    check('std::array<int, 58>' in hybrid_tests and 'expectedImport2L' in hybrid_tests
          and 'hybridIsDryDerivedComplement' in hybrid_tests and 'hybridIsHyperionFamily' in hybrid_tests
          and 'referenceFamilies' in hybrid_tests
          and 'lowerMode < kHybridFilterAlgorithmCount' in hybrid_tests
          and 'upperMode < kHybridFilterAlgorithmCount' in hybrid_tests
          and 'selectFilterRenderRoute(false, hybridNative, hybridNative)' in hybrid_tests
          and 'physicalControlRampSamples(48000.0) == 480' in hybrid_tests
          and 'unchanged K35 snapshots must not restart' in hybrid_tests
          and 'R-classic cutoff ramp' in hybrid_tests,
          "hybrid regression covers legacy map stability, appended HP response classification, route selection, and control ramps")
    check('dispatchFilterRenderRoute' in filter_route_tests
          and 'hybridK35Hp' in filter_route_tests and 'hybridR303Lp' in filter_route_tests
          and 'hybridRHuovilainenLp4' in filter_route_tests and 'hybridRDvalLp4' in filter_route_tests
          and 'independentRenderer == independentCalls' in filter_route_tests
          and 'verifyRepeatedSelectedSnapshots' in filter_route_tests
          and 'verifySelectedPhysicalOutputIsNotGenericClipped' in filter_route_tests
          and 'exceededFormerClipKnee' in filter_route_tests
          and 'real_detail::softClip' not in filter_route_tests,
          "direct route regression proves selected K35/R is independent, once-only, snapshot-continuous, and has no generic post-filter clip")
    check('#include "dsp/TrackFILT.inl"' in track_filter_route_tests
          and 'CountedPhysicalCore' in track_filter_route_tests
          and 'hybridK35Hp' in track_filter_route_tests and 'hybridRDvalLp4' in track_filter_route_tests
          and 'requireCounts(chain, 0, 0, 2 * kFrames' in track_filter_route_tests,
          "TrackFILT integration regression compiles the production body and excludes OLD/MNM around K35/R")
    check('testRepeatedModeSnapshots' in real_filter_tests and 'testTrueSampleRateChange' in real_filter_tests
          and 'kHybridRImport2LastAlgorithm' in real_filter_tests and 'kHybridFilterLastAlgorithm' in real_filter_tests,
          "real-filter regression covers R classic/R Import 2 callback cadence and actual sample-rate changes")
    check('schema23Hybrid' in rollback_tests and 'schema24Hybrid' in rollback_tests
          and 'schema25Hybrid' in rollback_tests and 'schema24Malformed' in rollback_tests
          and 'schema26ModeS' in rollback_tests and 'schema27ModeS' in rollback_tests
          and 'R DVAL' in rollback_tests,
          "state regression covers old-value protection, schema-24/25 filters, and schema-27 MODE S round trips")
    check('physical response' in import2_tests and 'common safety bound' in import2_tests
          and 'derived HP complement' in import2_tests and 'DVAL is dry plus LP' in import2_tests
          and 'open pass-band level' in import2_tests
          and 'unchanged R Import 2 host snapshot reset state' in import2_tests
          and '10 ms ramp' in import2_tests
          and 'case Import2Mode::DvalHp4: output = x + dval_.process(x);' in import2_filters
          and 'case Import2Mode::HuovilainenHp4: output = x - huovilainen_.process(x);' in import2_filters,
          "R Import 2 regression covers physical response, polarity-aware derived HP complements, safety, snapshots, and control ramps")
    check("VERSION 1.9.17" in cmake and 'version="1.9.17"' in jucer
          and 'versionString  = "1.9.17"' in generated_header
          and 'JucePlugin_VersionCode            0x10911' in generated_defines,
          "CMake, Projucer and generated metadata identify the 1.9.17 source package")
    check("ModeRollbackTests" in cmake and "NovaRollbackStateTests" in cmake
          and "DelayFeedbackDspTests" in cmake and "FmNewModeTests" in cmake
          and "FmFixModeTests" in cmake,
          "CMake registers rollback/state, delay-feedback, and isolated FM NEW/FIX regressions")
    check("FilterExtrasDspTests" in cmake and "ModEnvMatrixTests" in cmake
          and "MnmRealFilterTests" in cmake and "Import2FiltersTests" in cmake
          and "FilterRouteTests" in cmake and "TrackFilterRouteIntegrationTests" in cmake
          and "DistVariantTests" in cmake and "TrackDistVariantIntegrationTests" in cmake,
          "CMake registers FILT-extra, route-exclusivity, TrackFILT/TrackDIST integration, real-filter continuity, R Import 2, MODE S variants, and MOD ENV matrix regressions")
    check("ArpWindowTests" in cmake and "LfoMappingTests" in cmake,
          "CMake registers ARP-window and direct-LFO mapping regressions")
    if project.name.endswith("_FX"):
        check("project(MonomachineNovaUnifiedFX" in cmake
              and "BUNDLE_ID \"com.monomachinenova.unified.fx\"" in cmake
              and "PLUGIN_CODE NvFx" in cmake and "IS_SYNTH FALSE" in cmake
              and "NEEDS_MIDI_OUTPUT TRUE" in cmake and "VST3_CATEGORIES Fx" in cmake,
              "FX CMake metadata retains FX target, MIDI-out and VST3 identity")
    else:
        check("project(MonomachineNovaUnifiedSynth" in cmake
              and "BUNDLE_ID \"com.monomachinenova.unified.synth\"" in cmake
              and "PLUGIN_CODE NvSy" in cmake and "IS_SYNTH TRUE" in cmake
              and "NEEDS_MIDI_OUTPUT FALSE" in cmake and "VST3_CATEGORIES Instrument Synth" in cmake,
              "Synth CMake metadata retains Synth target and VST3 identity")

    if failures:
        print(f"\n{len(failures)} static integrity checks failed")
        return 1
    print("\nDSP/static integrity checks PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
