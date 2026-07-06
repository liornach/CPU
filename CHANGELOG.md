# Changelog

## Unreleased

### Removed
- `include/logic_gates.hpp` — an early, abandoned attempt at a gate hierarchy built on
  virtual interfaces (`IGate`, `IDualGate`) with a single `OrGate` implementation, plus
  `Input`/`Output` classes for wiring gate connections together. It never compiled as a
  whole (a missing semicolon after the `Input::Find` declaration is a syntax error), most
  of its methods were empty stubs, and nothing in the repository included it. It has been
  superseded by the plain-struct gate implementations in `include/gates/` (`NandGate`,
  `NotGate`, `AndGate`, `OrGate`, `NorGate`, `XorGate`), each with its own truth-table test.

  - Removed by: Claude Code
  - Commit: _pending_
