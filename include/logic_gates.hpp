
namespace CPU
{


struct Gate
{
    virtual ~Gate() = default;
    virtual bool eval() const = 0;
};


struct Input : public Gate
{
    explicit Input(bool v = false) : val_(v) {}
    bool eval() const override { return val_; }
    void set(bool v) { val_ = v; }

private:
    bool val_;
};


struct NandGate : public Gate
{
    NandGate(Gate* in1, Gate* in2) : a_(in1), b_(in2) {}
    bool eval() const override { return !(a_->eval() && b_->eval()); }

private:
    Gate* a_;
    Gate* b_;
};


struct NotGate : public Gate
{
    explicit NotGate(Gate* i) : in_(i), nand_(i, i) {}
    bool eval() const override { return nand_.eval(); }

private:
    Gate* in_;
    NandGate nand_;
};


struct AndGate : public Gate
{
    AndGate(Gate* a, Gate* b) : anandb_(a, b), nanandb_(&anandb_) {}
    bool eval() const override { return nanandb_.eval(); }

private:
    NandGate anandb_;
    NotGate  nanandb_;
};


struct OrGate : public Gate
{
    OrGate(Gate* a, Gate* b) : na_(a), nb_(b), and_(&na_, &nb_), not_(&and_) {}
    bool eval() const override { return not_.eval(); }

private:
    NotGate na_;
    NotGate nb_;
    AndGate and_;
    NotGate not_;
};


struct NorGate : public Gate
{
    NorGate(Gate* a, Gate* b) : aob_(a, b), naob_(&aob_) {}
    bool eval() const override { return naob_.eval(); }

private:
    OrGate  aob_;
    NotGate naob_;
};


struct AOIGate : public Gate
{
    AOIGate(Gate* a, Gate* b, Gate* c) : aab_(a, b), aab_oc_(&aab_, c), n_aab_oc_(&aab_oc_) {}
    bool eval() const override { return n_aab_oc_.eval(); }

private:
    AndGate aab_;
    OrGate  aab_oc_;
    NotGate n_aab_oc_;
};


struct XorGate : public Gate
{
    XorGate(Gate* a, Gate* b)
        : na_(a), nb_(b), naanb_(&na_, &nb_), aoi_abnaanb_(a, b, &naanb_) {}
    bool eval() const override { return aoi_abnaanb_.eval(); }

private:
    NotGate na_;
    NotGate nb_;
    AndGate naanb_;
    AOIGate aoi_abnaanb_;
};

} // namespace CPU
