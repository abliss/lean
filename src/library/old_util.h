/*
Copyright (c) 2014 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "kernel/environment.h"
#include "library/old_type_checker.h"

namespace lean {
typedef std::unique_ptr<old_type_checker> old_type_checker_ptr;

/** \brief Similar to previous procedure, but puts \c type in whnf */
expr to_telescope(old_type_checker & tc, expr type, buffer<expr> & telescope,
                  optional<binder_info> const & binfo = optional<binder_info>());
/** \brief Similar to previous procedure, but also accumulates constraints generated while
    normalizing type.

    \remark Constraints are generated only if \c type constains metavariables.
*/
expr to_telescope(old_type_checker & tc, expr type, buffer<expr> & telescope, optional<binder_info> const & binfo,
                  constraint_seq & cs);

expr mk_true();
bool is_true(expr const & e);
expr mk_true_intro();

bool is_and(expr const & e, expr & arg1, expr & arg2);
bool is_and(expr const & e);

expr mk_and(expr const & a, expr const & b);
expr mk_and_intro(old_type_checker & tc, expr const & Ha, expr const & Hb);
expr mk_and_elim_left(old_type_checker & tc, expr const & H);
expr mk_and_elim_right(old_type_checker & tc, expr const & H);

expr mk_poly_unit(level const & l);
expr mk_poly_unit_mk(level const & l);
expr mk_prod(old_type_checker & tc, expr const & A, expr const & B);
expr mk_pair(old_type_checker & tc, expr const & a, expr const & b);
expr mk_pr1(old_type_checker & tc, expr const & p);
expr mk_pr2(old_type_checker & tc, expr const & p);

expr mk_unit(level const & l, bool prop);
expr mk_unit_mk(level const & l, bool prop);
expr mk_prod(old_type_checker & tc, expr const & a, expr const & b, bool prop);
expr mk_pair(old_type_checker & tc, expr const & a, expr const & b, bool prop);
expr mk_pr1(old_type_checker & tc, expr const & p, bool prop);
expr mk_pr2(old_type_checker & tc, expr const & p, bool prop);

expr mk_false();
expr mk_empty();
/** \brief Return false (in standard mode) and empty (in HoTT) mode */
expr mk_false(environment const & env);

bool is_false(expr const & e);
bool is_empty(expr const & e);
/** \brief Return true iff \c e is false (in standard mode) or empty (in HoTT) mode */
bool is_false(environment const & env, expr const & e);
/** \brief Return an element of type t given an element \c f : false (in standard mode) and empty (in HoTT) mode */
expr mk_false_rec(old_type_checker & tc, expr const & f, expr const & t);

bool is_or(expr const & e);
bool is_or(expr const & e, expr & A, expr & B);

/** \brief Return true if \c e is of the form <tt>(not arg)</tt>, and store \c arg in \c a.
     Return false otherwise */
bool is_not(environment const & env, expr const & e, expr & a);
bool is_not(environment const & env, expr const & e);
expr mk_not(old_type_checker & tc, expr const & e);

/** \brief Create the term <tt>absurd e not_e : t</tt>. */
expr mk_absurd(old_type_checker & tc, expr const & t, expr const & e, expr const & not_e);

expr mk_eq(old_type_checker & tc, expr const & lhs, expr const & rhs);
expr mk_refl(old_type_checker & tc, expr const & a);
expr mk_symm(old_type_checker & tc, expr const & H);
expr mk_trans(old_type_checker & tc, expr const & H1, expr const & H2);
expr mk_subst(old_type_checker & tc, expr const & motive, expr const & x, expr const & y, expr const & xeqy, expr const & h);
expr mk_subst(old_type_checker & tc, expr const & motive, expr const & xeqy, expr const & h);
/** \brief Create an proof for x = y using subsingleton.elim (in standard mode) and is_trunc.is_prop.elim (in HoTT mode) */
expr mk_subsingleton_elim(old_type_checker & tc, expr const & h, expr const & x, expr const & y);

/** \brief Return true iff \c e is a term of the form (eq.rec ....) */
bool is_eq_rec_core(expr const & e);
/** \brief Return true iff \c e is a term of the form (eq.rec ....) in the standard library,
    and (eq.nrec ...) in the HoTT library. */
bool is_eq_rec(environment const & env, expr const & e);
/** \brief Return true iff \c e is a term of the form (eq.drec ....) in the standard library,
    and (eq.rec ...) in the HoTT library. */
bool is_eq_drec(environment const & env, expr const & e);

bool is_eq(expr const & e);
bool is_eq(expr const & e, expr & lhs, expr & rhs);
bool is_eq(expr const & e, expr & A, expr & lhs, expr & rhs);
/** \brief Return true iff \c e is of the form (eq A a a) */
bool is_eq_a_a(expr const & e);
/** \brief Return true iff \c e is of the form (eq A a a') where \c a and \c a' are definitionally equal */
bool is_eq_a_a(old_type_checker & tc, expr const & e);

bool is_heq(expr const & e);
bool is_heq(expr const & e, expr & A, expr & lhs, expr & B, expr & rhs);

bool is_ite(expr const & e, expr & c, expr & H, expr & A, expr & t, expr & f);
bool is_ite(expr const & e);

bool is_iff(expr const & e);
bool is_iff(expr const & e, expr & lhs, expr & rhs);
expr mk_iff(expr const & lhs, expr const & rhs);
expr mk_iff_refl(expr const & a);
/** \brief Given <tt>iff_pr : iff_term</tt>, where \c iff_term is of the form <tt>l <-> r</tt>,
    return the term <tt>propext l r iff_pr</tt>
*/
expr apply_propext(expr const & iff_pr, expr const & iff_term);

/** \brief If in HoTT mode, apply lift.down.
    The no_confusion constructions uses lifts in the proof relevant version (aka HoTT mode).
    We must apply lift.down to eliminate the auxiliary lift.
*/
optional<expr> lift_down_if_hott(old_type_checker & tc, expr const & v);

/** \brief Create a telescope equality for HoTT library.
    This procedure assumes eq supports dependent elimination.
    For HoTT, we can't use heterogeneous equality.
*/
void mk_telescopic_eq(old_type_checker & tc, buffer<expr> const & t, buffer<expr> const & s, buffer<expr> & eqs);
void mk_telescopic_eq(old_type_checker & tc, buffer<expr> const & t, buffer<expr> & eqs);

level mk_max(levels const & ls);

expr mk_sigma_mk(old_type_checker & tc, buffer<expr> const & ts, buffer<expr> const & as, constraint_seq & cs);

/** \brief Return true iff \c e is of the form (@option.none A), and update \c A */
bool is_none(expr const & e, expr & A);
/** \brief Return true iff \c e is of the form (@option.some A a), and update \c A and \c a */
bool is_some(expr const & e, expr & A, expr & a);

enum class implicit_infer_kind { Implicit, RelaxedImplicit, None };

/** \brief Infer implicit parameter annotations for the first \c nparams using mode
    specified by \c k.
*/
expr infer_implicit_params(expr const & type, unsigned nparams, implicit_infer_kind k);

/** \brief Similar to has_expr_metavar, but ignores metavariables occurring in the type
    of local constants */
bool has_expr_metavar_relaxed(expr const & e);

/** \brief Instantiate metavariables occurring in the expressions nested in \c c.

    \remark The justification associated with each assignment are *not* propagaged.
    We assume this is not a problem since we only used this procedure when connecting the
    elaborator with the tactic framework. */
constraint instantiate_metavars(constraint const & c, substitution & s);


/** \brief Check whether the given term is type correct or not, undefined universe levels are ignored,
    and untrusted macros are unfolded before performing the test.

    These procedures are useful for checking whether intermediate results produced by
    tactics and automation are type correct.
*/
void check_term(old_type_checker & tc, expr const & e);
void check_term(environment const & env, expr const & e);

/** \brief Return a justification for \c v_type being definitionally equal to \c t,
    <tt> v : v_type</tt>, the expressiong \c src is used to extract position information.
*/
format pp_type_mismatch(formatter const & fmt, expr const & v, expr const & v_type, expr const & t);
justification mk_type_mismatch_jst(expr const & v, expr const & v_type, expr const & t, expr const & src);
inline justification mk_type_mismatch_jst(expr const & v, expr const & v_type, expr const & t) {
    return mk_type_mismatch_jst(v, v_type, t, v);
}

/** \brief Create a type checker and normalizer that treats any constant named \c n as opaque when pred(n) is true.
    Projections are reduced using the projection converter */
old_type_checker_ptr mk_type_checker(environment const & env, name_predicate const & pred);
/** \brief Create a type checker and normalizer that treats any constant named \c n as opaque when pred(n) is true.
    No special support for projections is used */
old_type_checker_ptr mk_simple_type_checker(environment const & env, name_predicate const & pred);
/**
   \brief Generate the format object for <tt>hyps |- conclusion</tt>.
   The given substitution is applied to all elements.
*/
format format_goal(formatter const & fmt, buffer<expr> const & hyps, expr const & conclusion, substitution const & s);

/** \brief Given a metavariable application (?m l_1 ... l_n), apply \c s to the types of
    ?m and local constants l_i
    Return the updated expression and a justification for all substitutions.
*/
pair<expr, justification> update_meta(expr const & meta, substitution s);

/** \brief Instantiate metavariable application \c meta (?M ...) using \c subst  */
expr instantiate_meta(expr const & meta, substitution & subst);

/** \brief Return a 'failed to synthesize placholder' justification for the given
    metavariable application \c m of the form (?m l_1 ... l_k) */
justification mk_failed_to_synthesize_jst(environment const & env, expr const & m);

/** \brief Return true if it is a lean internal name, i.e., the name starts with a `_` */
bool is_internal_name(name const & n);

void initialize_library_old_util();
void finalize_library_old_util();
}