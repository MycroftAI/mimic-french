project('mimic-french', 'c', default_options : ['c_std=c99'], version: '0')

build_plugin = get_option('plugin')

sources = [
  'src/lang/fr_guess_pos.c',
  'src/lang/fr_init.c',
  'src/lang/fr_intonation.c',
  'src/lang/fr_lang_internal.h',
  'src/lang/fr_lex.c',
  'src/lang/fr_phoneset.c',
  'src/lang/fr_pos_tagger.c',
  'src/lang/fr_text_analysis.c',
  'src/lang/fr_tokenizer.c'
]

if get_option('rebuild_lex_lts')
  # This requires python3, festival and speech-tools (wagon, wfst_build)
  lex_and_lts = custom_target('lex_and_lts',
    output : ['fr_lex_data.c',
              'fr_lex_entries.c',
              'fr_lts_rules.c'],
    input : ['src/lex_src/fr_dict.out'],
    command : ['@0@/src/lex/fr/fr_make_lex'.format(meson.current_source_dir())])
else
  lex_and_lts = [
    'src/lex/fr_lex_data.c',
    'src/lex/fr_lex_entries.c',
    'src/lex/fr_lts_rules.c'
  ]
endif

includes = [include_directories('include')]

# Core Dependency:
mimiccore = dependency('ttsmimic_core', required: false)
if mimiccore.found()
  mimic_plugin_dir = mimiccore.get_pkgconfig_variable('mimic_plugin_dir')
else
  # We compile it:
  mimic_core_sub = subproject('mimic-core')
  mimiccore = mimic_core_sub.get_variable('ttsmimic_core_dep')
  mimic_plugin_dir = mimic_core_sub.get_variable('mimic_plugin_dir')
endif

# If mimic-core does not have plugin support, disable the plugin
if mimic_plugin_dir == ''
    build_plugin = false
endif
message('Build mimic-french plugin: @0@'.format(build_plugin))


fr_header_subdir = join_paths('ttsmimic', 'lang', 'fr')

ttsmimic_french_lib = library('ttsmimic_french', [sources, lex_and_lts],
                               include_directories : includes,
                               dependencies: [mimiccore],
                               install: true)

ttsmimic_french_dep = declare_dependency(link_with : ttsmimic_french_lib, 
                                          include_directories : includes)

if build_plugin
  ttsmimic_french_plugin_lib = library(
    'ttsmimic_french_plugin',
    ['src/mimic_french_plugin.c'],
    include_directories : includes,
    dependencies: [mimiccore, ttsmimic_french_dep],
    install_dir: mimic_plugin_dir,
    install: true)

  ttsmimic_french_plugin_dep = declare_dependency(
    link_with : ttsmimic_french_plugin_lib, 
    include_directories : includes)
endif



############ Headers #################

ttsmimic_headers = ['include/fr_lang.h', 'include/fr_lex.h']

install_headers(ttsmimic_headers, subdir : fr_header_subdir)

########### pkg-config #############

pkg = import('pkgconfig')

pkg.generate(libraries : ttsmimic_french_lib,
             name : 'ttsmimic_french',
             subdirs: fr_header_subdir,
             requires: ['ttsmimic_core'],
             description: 'French support for text to speech synthesis in mimic.',
             version: meson.project_version())

