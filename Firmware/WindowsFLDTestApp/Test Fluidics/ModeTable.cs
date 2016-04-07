 
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
[System.Xml.Serialization.XmlRootAttribute(Namespace="", ElementName="CytovilleFluidics", IsNullable =false)]
public class ModeTable {
    
    private CytovilleFluidicsMode[] modeField;
    
    private CytovilleFluidicsCalibrationTable calibrationTableField;
    
    private CytovilleFluidicsTimingTable timingTableField;
    
    [System.Xml.Serialization.XmlElementAttribute("Mode", Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsMode[] Mode {
        get {
            return this.modeField;
        }
        set {
            this.modeField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsCalibrationTable CalibrationTable {
        get {
            return this.calibrationTableField;
        }
        set {
            this.calibrationTableField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsTimingTable TimingTable {
        get {
            return this.timingTableField;
        }
        set {
            this.timingTableField = value;
        }
    }
}

[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsMode {
    
    private string modeNameField;
    
    private string descriptionField;
    
    private CytovilleFluidicsModeTimeSegment[] timeSegmentField;
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public string ModeName {
        get {
            return this.modeNameField;
        }
        set {
            this.modeNameField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public string Description {
        get {
            return this.descriptionField;
        }
        set {
            this.descriptionField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute("TimeSegment", Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegment[] TimeSegment {
        get {
            return this.timeSegmentField;
        }
        set {
            this.timeSegmentField = value;
        }
    }
}

[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsModeTimeSegment {
    
    private string descriptionField;
    
    private string timeField;
    
    private CytovilleFluidicsModeTimeSegmentValve valveField;
    
    private CytovilleFluidicsModeTimeSegmentPump pumpField;
    
    private CytovilleFluidicsModeTimeSegmentSample sampleField;
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public string Description {
        get {
            return this.descriptionField;
        }
        set {
            this.descriptionField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public string Time {
        get {
            return this.timeField;
        }
        set {
            this.timeField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegmentValve Valve {
        get {
            return this.valveField;
        }
        set {
            this.valveField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegmentPump Pump {
        get {
            return this.pumpField;
        }
        set {
            this.pumpField = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegmentSample Sample {
        get {
            return this.sampleField;
        }
        set {
            this.sampleField = value;
        }
    }
}

[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsModeTimeSegmentValve {
    
    private OnOff v1Field;
    
    private OnOff v2Field;
    
    private OnOff v3Field;
    
    private OnOff v4Field;
    
    private OnOff v5Field;
    
    private OnOff v6Field;
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V1 {
        get {
            return this.v1Field;
        }
        set {
            this.v1Field = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V2 {
        get {
            return this.v2Field;
        }
        set {
            this.v2Field = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V3 {
        get {
            return this.v3Field;
        }
        set {
            this.v3Field = value;
        }
    }
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V4 {
        get {
            return this.v4Field;
        }
        set {
            this.v4Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V5 {
        get {
            return this.v5Field;
        }
        set {
            this.v5Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff V6 {
        get {
            return this.v6Field;
        }
        set {
            this.v6Field = value;
        }
    }
}



[System.SerializableAttribute()]
public enum OnOff {
    
    
    On = 1,
    
    
    Off = 0,
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsModeTimeSegmentPump {
    
    private CytovilleFluidicsModeTimeSegmentPumpVACUUM vACUUMField;
    
    private OnOff p1Field;
    
    private OnOff p2Field;
    
    private OnOff p3Field;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegmentPumpVACUUM VACUUM {
        get {
            return this.vACUUMField;
        }
        set {
            this.vACUUMField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff P1 {
        get {
            return this.p1Field;
        }
        set {
            this.p1Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff P2 {
        get {
            return this.p2Field;
        }
        set {
            this.p2Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public OnOff P3 {
        get {
            return this.p3Field;
        }
        set {
            this.p3Field = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public enum CytovilleFluidicsModeTimeSegmentPumpVACUUM {
    
    
    Diff = 1,
    
    
    Off = 0,
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsModeTimeSegmentSample {
    
    private CytovilleFluidicsModeTimeSegmentSampleFlowRate flowRateField;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsModeTimeSegmentSampleFlowRate FlowRate {
        get {
            return this.flowRateField;
        }
        set {
            this.flowRateField = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public enum CytovilleFluidicsModeTimeSegmentSampleFlowRate {
    
    
    Hold = 4,
    
    
    High = 2,
    
    
    Med = 1,
    
    
    Low = 0,
    
    
    Dyn = 3,
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsCalibrationTable {
    
    private CytovilleFluidicsCalibrationTablePressure pressureField;
    
    private CytovilleFluidicsCalibrationTableSampleFlow sampleFlowField;
    
    private CytovilleFluidicsCalibrationTableSIT sITField;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsCalibrationTablePressure Pressure {
        get {
            return this.pressureField;
        }
        set {
            this.pressureField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsCalibrationTableSampleFlow SampleFlow {
        get {
            return this.sampleFlowField;
        }
        set {
            this.sampleFlowField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public CytovilleFluidicsCalibrationTableSIT SIT {
        get {
            return this.sITField;
        }
        set {
            this.sITField = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsCalibrationTablePressure {
    
    private float diffField;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float Diff {
        get {
            return this.diffField;
        }
        set {
            this.diffField = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsCalibrationTableSampleFlow {
    
    private float highField;
    
    private float medField;
    
    private float lowField;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float High {
        get {
            return this.highField;
        }
        set {
            this.highField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float Med {
        get {
            return this.medField;
        }
        set {
            this.medField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float Low {
        get {
            return this.lowField;
        }
        set {
            this.lowField = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsCalibrationTableSIT {
    
    private float sITUField;
    
    private float sITDField;
    
    private float sITPField;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float SITU {
        get {
            return this.sITUField;
        }
        set {
            this.sITUField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float SITD {
        get {
            return this.sITDField;
        }
        set {
            this.sITDField = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float SITP {
        get {
            return this.sITPField;
        }
        set {
            this.sITPField = value;
        }
    }
}



[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true)]
public class CytovilleFluidicsTimingTable {
    
    private float kT01Field;
    
    private float kT02Field;
    
    private float kT03Field;
    
    private float kT04Field;
    
    private float kT05Field;
    
    private float kT06Field;
    
    private float kT07Field;
    
    private float kT08Field;
    
    private float kT09Field;
    
    private float kT10Field;
    
    private float kT11Field;
    
    private float kT12Field;
    
    private float kT13Field;
    
    private float kT14Field;
    
    private float kT15Field;
    
    private float kT16Field;
    
    private float kT17Field;
    
    private float kT18Field;
    
    private float kT19Field;
    
    private float kT20Field;
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT01 {
        get {
            return this.kT01Field;
        }
        set {
            this.kT01Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT02 {
        get {
            return this.kT02Field;
        }
        set {
            this.kT02Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT03 {
        get {
            return this.kT03Field;
        }
        set {
            this.kT03Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT04 {
        get {
            return this.kT04Field;
        }
        set {
            this.kT04Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT05 {
        get {
            return this.kT05Field;
        }
        set {
            this.kT05Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT06 {
        get {
            return this.kT06Field;
        }
        set {
            this.kT06Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT07 {
        get {
            return this.kT07Field;
        }
        set {
            this.kT07Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT08 {
        get {
            return this.kT08Field;
        }
        set {
            this.kT08Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT09 {
        get {
            return this.kT09Field;
        }
        set {
            this.kT09Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT10 {
        get {
            return this.kT10Field;
        }
        set {
            this.kT10Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT11 {
        get {
            return this.kT11Field;
        }
        set {
            this.kT11Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT12 {
        get {
            return this.kT12Field;
        }
        set {
            this.kT12Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT13 {
        get {
            return this.kT13Field;
        }
        set {
            this.kT13Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT14 {
        get {
            return this.kT14Field;
        }
        set {
            this.kT14Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT15 {
        get {
            return this.kT15Field;
        }
        set {
            this.kT15Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT16 {
        get {
            return this.kT16Field;
        }
        set {
            this.kT16Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT17 {
        get {
            return this.kT17Field;
        }
        set {
            this.kT17Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT18 {
        get {
            return this.kT18Field;
        }
        set {
            this.kT18Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT19 {
        get {
            return this.kT19Field;
        }
        set {
            this.kT19Field = value;
        }
    }
    
    
    [System.Xml.Serialization.XmlElementAttribute(Form=System.Xml.Schema.XmlSchemaForm.Unqualified)]
    public float KT20 {
        get {
            return this.kT20Field;
        }
        set {
            this.kT20Field = value;
        }
    }
}
